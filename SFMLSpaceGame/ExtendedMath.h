#pragma once

#define _USE_MATH_DEFINES
#include <math.h>

#ifndef M_TAU
#define M_TAU (float)(M_PI + M_PI)
#endif

#ifndef M_PI_f
#define M_PI_f (float)M_PI
#endif

const float COS_5 = cosf(5.f * (M_PI_f / 180.f));
const float COS_15 = cosf(15.f * (M_PI_f / 180.f));
const float COS_30 = cosf(30.f * (M_PI_f / 180.f));
const float COS_45 = cosf(45.f * (M_PI_f / 180.f));
const float COS_60 = cosf(60.f * (M_PI_f / 180.f));

inline float ConstrainToPlusMinusPI(float angle)
{
	while (angle < -M_PI)
		angle += M_TAU;
	while (angle > M_PI)
		angle -= M_TAU;

	return angle;
}

inline sf::FloatRect Union(const sf::FloatRect& a, const sf::FloatRect& b) 
{
	float left = std::min(a.left, b.left);
	float top = std::min(a.top, b.top);
	float right = std::max((a.left + a.width), (b.left + b.width));
	float bottom = std::max((a.top + a.height), (b.top+ b.height));

	return sf::FloatRect(left, top, right - left, bottom - top);
}

inline float Lerp(float a, float b, float t)
{
	return a + ((b - a) * t);
}