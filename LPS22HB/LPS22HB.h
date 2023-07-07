/*
 * bmp581.h
 *
 *  Created on: Apr 16, 2023
 *      Author: kking
 */

#include "stm32f1xx_hal.h"
#include "math.h"
//#include <stdint.h>

#ifndef INC_LPS22HB_H_
#define INC_LPS22HB_H_

//Register map for the sensor
enum LPS_REG_MAP {
	CTRL_REG1 = 0x10,
	CTRL_REG2 = 0x11,
	CTRL_REG3 = 0x12,
	FIFO_CTRL = 0x14,
	REF_P_XL = 0x15,
	REF_P_L = 0x16,
	REF_P_H = 0x17,
	STATUS_REG = 0x27,
	PRESS_OUT_XL = 0x28,
	PRESS_OUT_L = 0x29,
	PRESS_OUT_H = 0x2A,
	TEMP_OUT_L = 0x2B,
	TEMP_OUT_H = 0x2C,
	LPFP_RES = 0x33
};

//ODR Options
enum LPS_ODR {
	ODR_off  = 0,
	ODR_1hz  = 0x1,
	ODR_10hz = 0x2,
	ODR_25hz = 0x3,
	ODR_50hz = 0x4,
	ODR_75hz = 0x5
};

//FIFO Options
enum LPS_FIFO {
	BYPASS = 0x0
};

enum LPS_LPFP_BANDWIDTH {
	ODR_2 = 0x0, //ODR/2
	ODR_9 = 0x2, //ODR/9
	ODR_20 = 0x3 //ODR/20
};

struct LPS_CONFIG {
	I2C_HandleTypeDef* i2c_config;
	uint16_t address;
};

#define LPS_DEFAULT_ADDRESS 0x5C

uint8_t LPS_Init(I2C_HandleTypeDef* i2c_config, uint16_t address);
void LPS_Reg_Read(uint16_t reg_addr, uint16_t reg_size, uint8_t* data_output);
void LPS_Set_Odr(enum LPS_ODR new_odr);
enum LPS_ODR LPS_Get_Odr();
void LPS_Configure_Fifo(enum LPS_FIFO desiredFifo);
void LPS_Configure_LPFP(enum LPS_LPFP_BANDWIDTH bandwidth);
double LPS_Get_Pressure();
double LPS_Get_Temp();
double LPS_Get_TempF();
double LPS_Get_PressureATM();
double LPS_Get_RelAlt_Ft(uint32_t reference_pressure);

#endif /* INC_LPS22HB_H_ */
