# BMP581 Library
The BMP581 Library provides an interface to read temperature and pressure data from a BMP581 sensor using an STM32 microcontroller. This library is written in C and can be used with any STM32 microcontroller that has I2C hardware support.
## Requirements
- STM32F1xx_HAL library
- math.h

## Installation
1. Download or clone the library to your STM32 project.
2. Add the BMP581 header file to your include path.
3. Include the BMP581 header file in your source code.
4. Initialize the BMP581 sensor using the provided functions.
5. Use the library functions to read temperature and pressure data from the sensor.

## Usage
### Initialization
Before using the BMP581 library, you must first initialize the sensor. There are two initialization functions available:
- `BMP_Init_Default_Addr(I2C_HandleTypeDef* i2c_config)` initializes the sensor at the default I2C address 71 (dec).
- `BMP_Init(I2C_HandleTypeDef* i2c_config, uint16_t address)` initializes the sensor at a specified I2C address.

## Reading Raw Register Data
- The `BMP_Reg_Read(uint16_t reg_addr, uint16_t reg_size, uint8_t* data_output)` function can be used to read data from the sensor's registers. The function takes the register address, the number of bytes to read, and a pointer to the output buffer.

## Switching Power Modes
- The `BMP_Switch_Power_Mode(enum BMP_Power_Mode new_mode)` function can be used to switch the sensor's power mode. The function takes a parameter of type `BMP_Power_Mode` which can be one of the following:
  - SLEEP
  - NORMAL
  - CONTINOUS

## Reading Data
The `BMP_Read_Data(uint32_t* temp_c, uint32_t* pressure)` function can be used to read the temperature and pressure data from the sensor. The function takes two pointers to uint32_t variables, which will hold the temperature and pressure data in Celsius and Pascals respectively.

## Getting Device Status
The `BMP_Get_Device_Status()` function returns the status of the sensor.

## Reading Temperature
The `BMP_Read_Temperature()` function returns the temperature data from the sensor.

## Reading Pressure
The `BMP_Read_Pressure()` function returns the pressure data from the sensor.

## Getting Relative Altitude
The `BMP_Get_RelAlt_Ft(uint32_t reference_pressure)` function can be used to calculate the relative altitude from a given reference pressure. The function takes a uint32_t parameter which represents the reference pressure in Pascals.

