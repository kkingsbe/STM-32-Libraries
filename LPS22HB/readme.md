# LPS22HB Library
The LPS22HB Library provides an interface to read temperature and pressure data from a LPS22HB sensor using an STM32 microcontroller. This library is written in C and can be used with any STM32 microcontroller that has I2C hardware support.
## Requirements
- STM32F1xx_HAL library
- math.h

## Installation
1. Download or clone the library to your STM32 project.
2. Add the LPS22HB header file to your include path.
3. Include the LPS22HB header file in your source code.
4. Initialize the LPS22HB sensor using the provided functions.
5. Use the library functions to read temperature and pressure data from the sensor.

## Quickstart
```
uint8_t lps_startup_result = LPS_Init(&hi2c1, LPS_DEFAULT_ADDRESS);
double refP = LPS_Get_Pressure();
double refT = LPS_Get_Temp();
double alt = 0.0;

//Main control loop
while (1)
{
  alt = LPS_Get_RelAlt_Ft(refP); //Get altitude in ft
}
```

## Usage
### Initialization
Before using the LPS22HB library, you must first initialize the sensor:
- `LPS_Init(I2C_HandleTypeDef* i2c_config, uint16_t address)` initializes the sensor at the default I2C address 71 (dec).

## Reading Raw Register Data
- The `LPS_Reg_Read(uint16_t reg_addr, uint16_t reg_size, uint8_t* data_output)` function can be used to read data from the sensor's registers. The function takes the register address, the number of bytes to read, and a pointer to the output buffer.

## Reading Temperature
The `LPS_Get_Temp()` function returns the temperature data from the sensor.

## Reading Pressure
The `LPS_Get_Pressure()` function returns the pressure data from the sensor.

## Getting Relative Altitude
The `LPS_Get_RelAlt_Ft(uint32_t reference_pressure)` function can be used to calculate the relative altitude from a given reference pressure. The function takes a uint32_t parameter which represents the reference pressure in HPA.


