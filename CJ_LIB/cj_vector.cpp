#include "cj_vector.h"


V2i v2i(i32 x, i32 y)
{
    V2i result = {};
    
    result.x = x;
    result.y = y;
    
    return result;
    
}

V3i v3i(i32 x, i32 y, i32 z)
{
    V3i result = {};
    
    result.x = x;
    result.y = y;
    result.z = z;
    
    return result;
    
}

V4i v4i(i32 x, i32 y, i32 z, i32 w)
{
    V4i result = {};
    
    result.x = x;
    result.y = y;
    result.z = z;
    result.w = w;
    
    return result;
    
}


V2f v2f(float x, float y)
{
    V2f result = {};
    
    result.x = x;
    result.y = y;
    
    return result;
    
}

V2f Scale_v2f(float scalar, V2f v)
{
	V2f result = {};

	result.x = scalar * v.x;
	result.y = scalar * v.y;

	return result;
}

V2f NormalTo_v2f(V2f v)
{
	V2f result = {};

	result.x = v.y;
	result.y = -v.x;

	return result;
}


V2f Unit_v2f(V2f v)
{
	V2f result = {};
	float length = Length_v2f(v);

	if(length > 0.0f)
	{
		result.x = v.x / length;
		result.y = v.y / length;
	}

	return result;
}

float Length_v2f(V2f v)
{
	float result = {};

	result = sqrt((v.x * v.x) + (v.y * v.y));

	return result;

}

float Dot_v2f(V2f a, V2f b)
{
	float result = {};
	V2f temp = {};

	temp.x = a.x * b.x;
	temp.y = a.y * b.y;

	result = temp.x + temp.y;
	
	return result;
}

V3f v3f(float x, float y, float z)
{
    V3f result = {};
    
    result.x = x;
    result.y = y;
    result.z = z;
    
    return result;
    
}

V4f v4f(float x, float y, float z, float w)
{
    V4f result = {};
    
    result.x = x;
    result.y = y;
    result.z = z;
    result.w = w;
    
    return result;
    
}






