#ifndef _CJ_RANDOM_H
#define _CJ_RANDOM_H

#include "stdlib.h"
#include "time.h"

enum CJ_RAND_STATE
{
	CJ_RAND_DONT_INCLUDE, CJ_RAND_INCLUDE
};

void CJ_InitRandom();
int CJ_RandIRange(int f_min, int f_max, int state);
float CJ_RandFRange(float f_min, float f_max, int state);

void CJ_InitRandom()
{
	srand(time(NULL));
}

int CJ_RandIRange(int f_min, int f_max, int state)
{
	int result = 0.0f;
	int min;
	int max;
	int num_range;

	if(f_min > f_max)
	{
		int temp;
		temp = f_min;
		f_min = f_max;
		f_max = temp;
	}

	min = f_min;
	switch(state)
	{
		case CJ_RAND_DONT_INCLUDE:
			max = f_max; break;
		case CJ_RAND_INCLUDE:
			max = f_max + 1; break;
	}

	num_range = max - min;

	result = min + (rand() % num_range);

	return result;
}

float CJ_RandFRange(float f_min, float f_max, int state)
{
	float result = 0.0f;
	float min;
	float max;
	float num_range;

	if(f_min > f_max)
	{
		float temp;
		temp = f_min;
		f_min = f_max;
		f_max = temp;
	}

	min = f_min;
	switch(state)
	{
		case CJ_RAND_DONT_INCLUDE:
			max = f_max; break;
		case CJ_RAND_INCLUDE:
			max = f_max + 1; break;
	}

	num_range = max - min;

	result = min + (float)(rand() / (float)(RAND_MAX / num_range));

	return result;
}


#endif
