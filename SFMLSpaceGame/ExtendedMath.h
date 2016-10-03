#pragma once

#define _USE_MATH_DEFINES
#include <math.h>

#ifndef M_TAU
#define M_TAU (float)(M_PI + M_PI)
#endif

inline float ConstrainToPlusMinusPI(float angle)
{
	while (angle < -M_PI)
		angle += M_TAU;
	while (angle > M_PI)
		angle -= M_TAU;

	return angle;
}
