/*
 * LPS22HB.c
 *
 *  Created on: Jul 6, 2023
 *      Author: Kyle Kingsberry
 */
#include "LPS22HB.h"

struct LPS_CONFIG lpsConfig;

uint8_t LPS_Init(I2C_HandleTypeDef* i2c_config, uint16_t address)
{
	HAL_Delay(1000);
	lpsConfig.i2c_config = i2c_config;
	lpsConfig.address = address;

	//Make sure the sensor is powered on and discovered on the I2C bus
	HAL_StatusTypeDef i2c_status = HAL_I2C_IsDeviceReady(i2c_config, (uint16_t)(LPS_DEFAULT_ADDRESS<<1), 3, 5);
	if(i2c_status == HAL_BUSY) {
		return 0;
	}

	//Set the ouptut data rate (odr)
	LPS_Set_Odr(ODR_75hz);

	//Make sure the odr matches the desired odr
	enum LPS_ODR odr = LPS_Get_Odr();
	if(odr != ODR_75hz) {
		return 0;
	}

	//Set FIFO mode to BYPASS
	LPS_Configure_Fifo(BYPASS);

	//Enable low pass filter
	LPS_Configure_LPFP(ODR_9);

	return 1;
}

void LPS_Reg_Read(uint16_t reg_addr, uint16_t reg_size, uint8_t* data_output)
{
	HAL_I2C_Mem_Read(lpsConfig.i2c_config, (uint16_t)(lpsConfig.address<<1), reg_addr, 1, data_output, reg_size, 100);
}

//Configures the output data rate (ODR)
void LPS_Set_Odr(enum LPS_ODR new_odr)
{
	//Read the existing configuration from the CTRL_REG 1
	uint8_t existingConfig[1];
	LPS_Reg_Read(CTRL_REG1, 1, existingConfig);

	//Clear top 4 bits from the existing config
	uint8_t newConfig[1];
	newConfig[0] = existingConfig[0] & 0x0F;

	//Store the new ODR configuration
	newConfig[0] |= (new_odr << 4);

	//Write new ODR to register
	HAL_I2C_Mem_Write(lpsConfig.i2c_config, (uint16_t)(lpsConfig.address<<1), CTRL_REG1, 1, newConfig, 1, 100);

	/*
	uint8_t actualNewConfig[1];
	LPS_Reg_Read(CTRL_REG1, 1, actualNewConfig);
	*/
}

//Reads the sensors odr
enum LPS_ODR LPS_Get_Odr()
{
	uint8_t existingConfig[1];
	LPS_Reg_Read(CTRL_REG1, 1, existingConfig);
	uint8_t odr_code = (existingConfig[0] >> 4);

	return odr_code;
}

//Configures the FIFO for the desired mode
void LPS_Configure_Fifo(enum LPS_FIFO desiredFifo)
{
	//Only supports bypass mode for now
	if(desiredFifo != BYPASS) {
		return;
	}

	//Read existing FIFO config from sensor
	uint8_t existingConfig[1];
	LPS_Reg_Read(FIFO_CTRL, 1, existingConfig);

	//Clear the top 3 bits from the existing config
	uint8_t newConfig[1];
	newConfig[0] = existingConfig[0] & 0x1F;

	//Write new FIFO config to sensor
	HAL_I2C_Mem_Write(lpsConfig.i2c_config, (uint16_t) (lpsConfig.address<<1), FIFO_CTRL, 1, newConfig, 1, 100);
}

//Returns the pressure read by the sensor in HPA
double LPS_Get_Pressure()
{
	double SCALING_FACTOR = 4096.0;
	uint8_t sink[1];
	LPS_Reg_Read(LPFP_RES, 1, sink); //Reset low pass filter

	uint8_t press_out_h[1];
	uint8_t press_out_l[1];
	uint8_t press_out_xl[1];

	LPS_Reg_Read(PRESS_OUT_H, 1, press_out_h);
	LPS_Reg_Read(PRESS_OUT_L, 1, press_out_l);
	LPS_Reg_Read(PRESS_OUT_XL, 1, press_out_xl);

	uint32_t pressure = (press_out_h[0] << 16) + (press_out_l[0] << 8) + press_out_xl[0];

	return pressure / SCALING_FACTOR;
}

//Returns the temperature read by the sensor in C
double LPS_Get_Temp()
{
	double SCALING_FACTOR = 100.0;
	uint8_t temp_out_h[1];
	uint8_t temp_out_l[1];

	LPS_Reg_Read(TEMP_OUT_H, 1, temp_out_h);
	LPS_Reg_Read(TEMP_OUT_L, 1, temp_out_l);

	uint32_t temperature = (temp_out_h[0] << 8) + temp_out_l[0];

	return temperature / SCALING_FACTOR;
}

double LPS_Get_TempF()
{
	uint16_t tempC = LPS_Get_Temp();
	return (tempC * (9/5.0)) + 32;
}

double LPS_Get_PressureATM()
{
	uint16_t pressHPA = LPS_Get_Pressure();
	return pressHPA * 0.0009869233;
}

//Reference pressure is pressure in Pa at surface
double LPS_Get_RelAlt_Ft(uint32_t reference_pressure)
{
	uint16_t p = LPS_Get_Pressure();
	uint16_t t = LPS_Get_Temp();

	//Hypsometric formula: https://keisan.casio.com/exec/system/1224585971
	double frac_p = (double)reference_pressure / p;
	double exponential = pow(frac_p, (double)1.0/5.257);
	double fraction_top = (exponential - 1) * (t + 273.15);
	double alt_m = fraction_top / 0.0065;
	return alt_m * 3.281; //Convert to ft and return
}

/*
void LPS_Configure_LPFP(enum LPS_LPFP_BANDWIDTH bandwidth)
{
	//Read the existing configuration from the CTRL_REG 1
	uint8_t existingConfig[1];
	LPS_Reg_Read(CTRL_REG1, 1, existingConfig);

	uint8_t newConfig[1];
	newConfig[0] = existingConfig[0] & 0xC;

	//Store the new ODR configuration
	newConfig[0] |= (bandwidth << 2);

	//Write new ODR to register
	HAL_I2C_Mem_Write(lpsConfig.i2c_config, (uint16_t)(lpsConfig.address<<1), CTRL_REG1, 1, newConfig, 1, 100);
}
*/
