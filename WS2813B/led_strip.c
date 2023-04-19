/*
 * led_strip.c
 *
 *  Created on: Apr 15, 2023
 *      Author: kking
 */

#include "led_strip.h"

void Strip_Set_Timer(TIM_HandleTypeDef* timer)
{
	Set_LED_Timer(timer);
}

void Strip_Progress_Bar_Single_Color(uint8_t num_filled, enum COLOR color)
{
	Clear_All_LED();
	if(num_filled > 5) num_filled = 5;
	for(uint8_t i = 0; i < num_filled; i++) {
		Set_LED_From_Color_Name(i, color);
	}
}

void Strip_Clear()
{
	Clear_All_LED();
}

void Strip_Send()
{
	WS2813_Send();
}
