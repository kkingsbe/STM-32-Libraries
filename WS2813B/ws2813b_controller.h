/*
 * ws2813b.h
 *
 *  Created on: Apr 14, 2023
 *      Author: kking
 */

#include "stm32f1xx_hal.h"
#include "math.h"

#ifndef INC_WS2813B_H_
#define INC_WS2813B_H_

#define MAX_LED 5
#define USE_BRIGHTNESS 1

struct WS2813B_Config {
	TIM_HandleTypeDef* timer;
	uint8_t LED_Data[MAX_LED][4];
	uint8_t LED_Mod[MAX_LED][4];
	uint16_t pwmData[(24*MAX_LED) + 50];
	uint8_t datasentflag;
};

struct RGB {
	uint8_t r;
	uint8_t g;
	uint8_t b;
};

enum COLOR {
	GREEN,
	YELLOW,
	RED,
	PURPLE,
	SKYBLUE,
	CLEAR
};

extern struct WS2813B_Config ws_config;

void Set_LED_Timer(TIM_HandleTypeDef* timer);
void Set_LED (int LEDnum, int Red, int Green, int Blue);
void Set_Brightness (int brightness);  // 0-45
void WS2813_Send ();
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim);
void Color_To_RGB(struct RGB* rgb, enum COLOR color);
void Set_LED_From_RGB(int LEDnum, struct RGB* rgb);
void Set_LED_From_Color_Name(int LEDnum, enum COLOR color);
void Clear_All_LED();

#endif /* INC_WS2813B_H_ */
