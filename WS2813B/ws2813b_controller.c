/*
 * ws2813b.cpp
 *
 *  Created on: Apr 14, 2023
 *      Author: kking
 */
#include <ws2813b_controller.h>
/*
uint8_t LED_Data[MAX_LED][4];
uint8_t LED_Mod[MAX_LED][4];  // for brightness

uint16_t pwmData[(24*MAX_LED)+50];
uint8_t datasentflag = 0;
*/

struct WS2813B_Config ws_config;

void Set_LED_Timer(TIM_HandleTypeDef* timer)
{
	ws_config.timer = timer;
}

void Set_LED (int LEDnum, int Red, int Green, int Blue)
{
	ws_config.LED_Data[LEDnum][0] = LEDnum;
	ws_config.LED_Data[LEDnum][1] = Green;
	ws_config.LED_Data[LEDnum][2] = Red;
	ws_config.LED_Data[LEDnum][3] = Blue;
}

#define PI 3.14159265

void Set_Brightness (int brightness)  // 0-45
{
#if USE_BRIGHTNESS

	if (brightness > 45) brightness = 45;
	for (int i=0; i<MAX_LED; i++)
	{
		ws_config.LED_Mod[i][0] = ws_config.LED_Data[i][0];
		for (int j=1; j<4; j++)
		{
			float angle = 90-brightness;  // in degrees
			angle = angle*PI / 180;  // in rad
			ws_config.LED_Mod[i][j] = (ws_config.LED_Data[i][j])/(tan(angle));
		}
	}

#endif

}

void WS2813_Send ()
{
	uint32_t indx=0;
	uint32_t color;


	for (int i= 0; i<MAX_LED; i++)
	{
#if USE_BRIGHTNESS
		color = ((ws_config.LED_Mod[i][1]<<16) | (ws_config.LED_Mod[i][2]<<8) | (ws_config.LED_Mod[i][3]));
#else
		color = ((ws_config.LED_Data[i][1]<<16) | (ws_config.LED_Data[i][2]<<8) | (ws_config.LED_Data[i][3]));
#endif

		for (int i=23; i>=0; i--)
		{
			if (color&(1<<i))
			{
				ws_config.pwmData[indx] = 48;  // 75% of 64
			}

			else ws_config.pwmData[indx] = 20;  // ~30% of 64

			indx++;
		}

	}

	//May need to be changed
	for (int i=0; i<50; i++)
	{
		ws_config.pwmData[indx] = 0;
		indx++;
	}

	HAL_TIM_PWM_Start_DMA(ws_config.timer, TIM_CHANNEL_2, (uint32_t *)ws_config.pwmData, indx);
	while (!ws_config.datasentflag){};
	ws_config.datasentflag = 0;
}

void Color_To_RGB(struct RGB* rgb, enum COLOR color)
{
	if(color == GREEN) {
		rgb->r = 0;
		rgb->g = 255;
		rgb->b = 20;
	}

	if(color == YELLOW) {
		rgb->r = 255;
		rgb->g = 255;
		rgb->b = 0;
	}

	if(color == RED) {
		rgb->r = 255;
		rgb->g = 0;
		rgb->b = 0;
	}

	if(color == PURPLE) {
		rgb->r = 255;
		rgb->g = 0;
		rgb->b = 255;
	}

	if(color == SKYBLUE) {
		rgb->r = 0;
		rgb->g = 255;
		rgb->b = 200;
	}

	if(color == CLEAR) {
		rgb->r = 0;
		rgb->g = 0;
		rgb->b = 0;
	}
}

void Set_LED_From_RGB(int LEDnum, struct RGB* rgb)
{
	Set_LED(LEDnum, rgb->r, rgb->g, rgb->b);
}

void Set_LED_From_Color_Name(int LEDnum, enum COLOR color)
{
	struct RGB rgb;
	Color_To_RGB(&rgb, color);
	Set_LED_From_RGB(LEDnum, &rgb);
}

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
	HAL_TIM_PWM_Stop_DMA(ws_config.timer, TIM_CHANNEL_2);
	ws_config.datasentflag=1;
}

void Clear_All_LED()
{
	for(uint8_t i = 0; i < MAX_LED; i++)
	{
		Set_LED_From_Color_Name(i, CLEAR);
	}
}
