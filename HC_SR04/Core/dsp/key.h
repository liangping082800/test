#ifndef __KEY_H__
#define __KEY_H__


#include "main.h"
#include "stdbool.h"


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);

struct keys
{
	uint8_t jude_sta;
	bool key_sta;
	bool single_flag;
	bool long_flag;
	unsigned int ket_time;
};


extern struct keys key[4];

#endif
