WIP, use at your own risk. Only works with 5 leds currently because that is all I needed it to work with. Will revisit when I have time. led_strip.h provides an abstraction layer over ws2813b_controller.h

Example:
```
#include "led_strip.h"
Strip_Set_Timer(&htim2);
Strip_Clear();
uint16_t j = 0;
uint16_t k = 0;
while (1)
{
  Set_Brightness(5);
  if(k == 0) {
    Strip_Progress_Bar_Single_Color((j/20)%6, RED);
  } else if(k == 1) {
    Strip_Progress_Bar_Single_Color((j/20)%6, YELLOW);
  } else if(k == 2) {
    Strip_Progress_Bar_Single_Color((j/20)%6, GREEN);
  } else if(k == 3) {
    Strip_Progress_Bar_Single_Color((j/20)%6, SKYBLUE);
  } else if(k == 4) {
    Strip_Progress_Bar_Single_Color((j/20)%6, PURPLE);
  }


  if(j%120 == 0) {
    k++;
    if(k > 4) k = 0;
  }
  Strip_Send();
  HAL_Delay (10);
  j++;
}
```
