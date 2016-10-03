#pragma once

#define _USE_MATH_DEFINES
#include <math.h>

#ifndef M_TAU
#define M_TAU (float)(M_PI + M_PI)
#endif

const float COS_5 = cos(5.f * (M_PI / 180.f));
const float COS_15 = cos(15.f * (M_PI / 180.f));
const float COS_30 = cos(30.f * (M_PI / 180.f));

inline float ConstrainToPlusMinusPI(float angle)
{
	while (angle < -M_PI)
		angle += M_TAU;
	while (angle > M_PI)
		angle -= M_TAU;

	return angle;
}
