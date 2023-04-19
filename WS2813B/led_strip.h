/*
 * led_strip.h
 *
 *  Created on: Apr 15, 2023
 *      Author: kking
 */

#include <ws2813b_controller.h>

#ifndef INC_LED_STRIP_H_
#define INC_LED_STRIP_H_

void Strip_Set_Timer(TIM_HandleTypeDef* timer);
void Strip_Progress_Bar_Single_Color(uint8_t num_filled, enum COLOR color);
void Strip_Clear();
void Strip_Send();

#endif /* INC_LED_STRIP_H_ */
