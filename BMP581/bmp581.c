/*
 * bmp581.c
 *
 *  Created on: Apr 16, 2023
 *      Author: kking
 */
#include "bmp581.h"

struct BMP_Config bmpConfig;
enum BMP_Measurement_Mode bmpCurrentMode;

void BMP_Init_Default_Addr(I2C_HandleTypeDef* i2c_config)
{
	BMP_Init(i2c_config, BMP_DEFAULT_ADDRESS);
}

void BMP_Init(I2C_HandleTypeDef* i2c_config, uint16_t address)
{
	HAL_Delay(1000);
	bmpConfig.i2c_config = i2c_config;
	bmpConfig.address = address;
	bmpCurrentMode = NONE;

	//Enter sleep mode
	BMP_Switch_Power_Mode(SLEEP);

	//Configure FIFO
	BMP_Configure_FIFO();

	//Enter normal mode
	BMP_Switch_Power_Mode(NORMAL);
}

void BMP_Configure_FIFO()
{
	//Configure FIFO
	uint8_t fifo_config [1];
	BMP_Reg_Read(BMP_FIFO_SEL, 1, fifo_config);
	fifo_config[0] &= 0xFC; //Clear first 2 bits
	fifo_config[0] |= 0x3;  //Set first 2 bits to 0x3 (pt mode)
	HAL_I2C_Mem_Write(bmpConfig.i2c_config, (uint16_t)(bmpConfig.address<<1), BMP_FIFO_SEL, 1, fifo_config, 1, 5);
	HAL_Delay(10);
}

void BMP_Switch_Power_Mode(enum BMP_Power_Mode new_mode)
{
	uint8_t odr_config[1];
	BMP_Reg_Read(BMP_ODR_CONFIG, 1, odr_config);
	odr_config[0] &= 0xFC; // Clear the lower 2 bits

	uint8_t mode_code; //Stores the 2 bit mode code

	if(new_mode == SLEEP)
	{
		mode_code = 0x00;
	}
	if(new_mode == NORMAL)
	{
		mode_code = 0x1;
	}

	odr_config[0] |= mode_code; // Set the lower 2 bits to the desired value

	//Write new power mode configuration
	HAL_I2C_Mem_Write(bmpConfig.i2c_config, (uint16_t)(bmpConfig.address<<1), BMP_ODR_CONFIG, 1, odr_config, 1, 5);
	HAL_Delay(10);

	//Wait for mode change to apply
	uint8_t current_odr_config[1];
	BMP_Reg_Read(BMP_ODR_CONFIG, 1, current_odr_config);
	uint8_t pm_mode = current_odr_config[0] & 0x3; //Mask out all but the first 2 bits

	while(pm_mode != mode_code)
	{
		BMP_Reg_Read(BMP_ODR_CONFIG, 1, current_odr_config);
		pm_mode = current_odr_config[0] & 0x3;
	}

	HAL_Delay(10);
}

void BMP_Reg_Read(uint16_t reg_addr, uint16_t reg_size, uint8_t* data_output)
{
	HAL_I2C_Mem_Read(bmpConfig.i2c_config, (uint16_t)(bmpConfig.address<<1), reg_addr, 1, data_output, reg_size, 100);
}

uint8_t BMP_Get_Device_Status()
{
	HAL_StatusTypeDef i2c_status = HAL_I2C_IsDeviceReady(bmpConfig.i2c_config, (uint16_t)(bmpConfig.address<<1), 3, 5);

	//HAL_Delay(10);
	uint8_t chipid_rx_dat[1];
	BMP_Reg_Read(BMP_CHIP_ID_REG, 1, chipid_rx_dat);
	uint8_t chip_id = chipid_rx_dat[0];

	uint8_t status_rx_dat[1];
	BMP_Reg_Read(BMP_STATUS_REG, 1, status_rx_dat);
	uint8_t bmp_status = status_rx_dat[0];

	if(i2c_status == HAL_OK && bmp_status == 0x2)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void BMP_Read_Data(uint16_t* temp_c, uint16_t* pressure)
{
	uint8_t data[6];
	BMP_Reg_Read(BMP_FIFO_DATA, 6, data);
	uint32_t temp_conc = (uint32_t)(data[2] << 16 | data[1] << 8 | data[0]);
	*temp_c = temp_conc >> 16;

	uint32_t pressure_conc = (uint32_t)(data[5] << 16 | data[4] << 8 | data[3]);
	*pressure = pressure_conc >> 16;
}

uint16_t BMP_Read_Temperature()
{
	uint8_t temp_arr[6];
	BMP_Reg_Read(BMP_FIFO_DATA, 6, temp_arr);
	uint32_t temp_conc = (uint32_t)(temp_arr[2] << 16 | temp_arr[1] << 8 | temp_arr[0]);
	return temp_conc >> 16;
}

uint16_t BMP_Read_Pressure()
{
	uint8_t temp_arr[6];
	BMP_Reg_Read(BMP_FIFO_DATA, 6, temp_arr);
	uint32_t temp_conc = (uint32_t)(temp_arr[2] << 16 | temp_arr[1] << 8 | temp_arr[0]);
	return temp_conc >> 16;
}

enum BMP_Power_Mode BMP_Get_Mode()
{
	uint8_t odr_config[1];
	BMP_Reg_Read(BMP_ODR_CONFIG, 1, odr_config);
	uint8_t pm_bin = odr_config[0];
	pm_bin &= 0x3; //Mask out all bits except for the first 2 (binary 11 or 0x3)

	if(pm_bin == 0x0)
	{
		return STANDBY;
	}

	if(pm_bin == 0x1)
	{
		return NORMAL;
	}

	if(pm_bin == 0x70)
	{
		return SLEEP;
	}
}





