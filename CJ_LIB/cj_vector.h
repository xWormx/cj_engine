/* date = October 24th 2020 11:01 pm */

#ifndef CJ_VECTOR_H
#define CJ_VECTOR_H

#include "cj_int.h"
#include "math.h"

struct V2i
{
    i32 x;
    i32 y;

};

struct V3i
{
    i32 x;
    i32 y;
    i32 z;
};

struct V4i
{
    i32 x;
    i32 y;
    i32 z;
    i32 w;
};

struct V2f
{
    float x;
    float y;

    V2f operator-(V2f v)
    {
	    V2f result = {};
	    
	    result.x = this->x - v.x;
	    result.y = this->y - v.y;

	    return result;
    }

    void operator+=(V2f v)
    {
	    this->x += v.x;
	    this->y += v.y;
    }

    void operator-=(V2f v)
    {
	    this->x -= v.x;
	    this->y -= v.y;
    }
};

struct V3f
{
    float x;
    float y;
    float z;
};

struct V4f
{
    float x;
    float y;
    float z;
    float w;
};

struct V2d
{
    double x;
    double y;

    V2d operator-(V2d v)
    {
	    V2d result = {};
	    
	    result.x = this->x - v.x;
	    result.y = this->y - v.y;

	    return result;
    }

    void operator+=(V2d v)
    {
	    this->x += v.x;
	    this->y += v.y;
    }

    void operator-=(V2d v)
    {
	    this->x -= v.x;
	    this->y -= v.y;
    }
};




V2i v2i(i32 x, i32 y);
V3i v3i(i32 x, i32 y, i32 z);
V4i v4i(i32 x, i32 y, i32 z, i32 w);


V2f v2f(float x, float y);
V2f Scale_v2f(float scalar, V2f v);
V2f NormalTo_v2f(V2f v);
V2f Unit_v2f(V2f v);
float Length_v2f(V2f v);
float Dot_v2f(V2f a, V2f b);


V3f v3f(float x, float y, float z);
V4f v4f(float x, float y, float z, float w);

V2d v2d(double x, double y);
#endif //CJ_VECTOR_H
