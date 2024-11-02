#include "hc_sr04.h"
int upEdge = 0;
int downEdge = 0;
float distance = 0;
/*捕获电平时间*/
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    if(htim==&htim2 && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
    {
      upEdge = HAL_TIM_ReadCapturedValue(&htim2,TIM_CHANNEL_1);
			downEdge = HAL_TIM_ReadCapturedValue(&htim2,TIM_CHANNEL_2);
			distance = ((downEdge - upEdge) * 0.034)/2;
    }
}
