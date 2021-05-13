#ifndef _CJ_MATH_
#define _CJ_MATH_

#define CJ_PI 3.14159265f

float Min_f(float *list, u32 n_elements);
float Max_f(float *list, u32 n_elements);

float Overlap_f(float minA, float maxA, float minB, float maxB);

float Min_f(float *list, u32 n_elements)
{
	float result = list[0];
	for(i32 i = 0; i < n_elements; i++)
	{
		float current = list[i];
		if(current < result)
		{
			result = current;
		}


	}

	return result;
}


float Max_f(float *list, u32 n_elements)
{
	float result = list[0];
	for(i32 i = 0; i < n_elements; i++)
	{
		float current = list[i];
		if(current > result)
		{
			result = current;
		}


	}

	return result;
}

float Overlap_f(float minA, float maxA, float minB, float maxB)
{
	float overlap = 0.0f;
	float min = 0.0f;
	float max = 0.0f;


	if(minA < minB)
	{
		min = minB;
	}
	else
	{
		min = minA;
	}

	if(maxA > maxB)
	{
		max = maxB;
	}
	else
	{
		max = maxA;
	}

	overlap = max - min;
	

	return overlap;
}

#endif
