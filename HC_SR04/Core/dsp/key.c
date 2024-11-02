#include "key.h"

struct keys key[4]={0,0,0};

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance==TIM1)
	{
		key[0].key_sta=HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_12);
		key[1].key_sta=HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_13);
		key[2].key_sta=HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_14);
		key[3].key_sta=HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_15);
		
		for(int i=0;i<4;i++)
		{
			switch(key[i].jude_sta)
			{
				case 0:
				{
					if(key[i].key_sta==0) 
					{
						key[i].jude_sta=1;
						key[i].ket_time=0;
					}
				}
				break;
				 
				case 1:
				{
					if(key[i].key_sta==0)
					{
						key[i].jude_sta=2;
						
						
					}
   			else key[i].jude_sta=0;			
				}
				break;
				case 2:
				{
					if(key[i].key_sta==1)
					{
						key[i].jude_sta=0;
						
						if(key[i].ket_time<70)
						{
							key[i].single_flag=1;
						}
					}
					else 
					{
						
						key[i].ket_time++;
						if(key[i].ket_time>70) 
						{
							key[i].long_flag=1;
							key[i].ket_time=0;
							
						}
					}  					
				}
				break;
				
			}
		}
	}
}
