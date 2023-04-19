/*
 * bmp581.h
 *
 *  Created on: Apr 16, 2023
 *      Author: kking
 */

#include "stm32f1xx_hal.h"
//#include <stdint.h>

#ifndef INC_BMP581_H_
#define INC_BMP581_H_

#define BMP_DEFAULT_ADDRESS 71
#define BMP_TEMP_MSB_REG 0x1F
#define BMP_TEMP_LSB_REG 0x1E
#define BMP_TEMP_XLSB_REG 0x1D
#define BMP_STATUS_REG 0x28
#define BMP_CHIP_ID_REG 0x01
#define BMP_FIFO_SEL 0x18
#define BMP_FIFO_DATA 0x29
#define BMP_ODR_CONFIG 0x37

struct BMP_Config {
	I2C_HandleTypeDef* i2c_config;
	uint16_t address;
};

enum BMP_Measurement_Mode {
	NONE,
	TEMPERATURE,
	PRESSURE
};

enum BMP_Power_Mode {
	STANDBY,
	SLEEP,
	NORMAL
};

extern struct BMP_Config bmpConfig;
extern enum BMP_Measurement_Mode bmpCurrentMode;

void BMP_Init_Default_Addr(I2C_HandleTypeDef* i2c_config);
void BMP_Init(I2C_HandleTypeDef* i2c_config, uint16_t address);
void BMP_Reg_Read(uint16_t reg_addr, uint16_t reg_size, uint8_t* data_output);
void BMP_Switch_Power_Mode(enum BMP_Power_Mode new_mode);
enum BMP_Power_Mode BMP_Get_Mode();
void BMP_Configure_FIFO();
void BMP_Read_Data(uint16_t* temp_c, uint16_t* pressure);

uint8_t BMP_Get_Device_Status();
uint16_t BMP_Read_Temperature();
uint16_t BMP_Read_Pressure();

#endif /* INC_BMP581_H_ */
