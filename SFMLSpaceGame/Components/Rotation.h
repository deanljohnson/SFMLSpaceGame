#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
#include <Components/Component.h>

#ifndef DEG_TO_RAD
#define DEG_TO_RAD(__val__) __val__ * (float)M_PI / 180.f
#endif

#ifndef RAD_TO_DEG
#define RAD_TO_DEG(__val__) __val__ *  180.f / (float)M_PI
#endif

class Rotation : public Component
{
private:
	float m_radians{ 0.f };
	float m_degrees{ 0.f };
public:
	float GetDegrees() const { return m_degrees; }
	float GetRadians() const { return m_radians; }

	void SetDegrees(float d) 
	{
		m_degrees = d;
		m_radians = DEG_TO_RAD(d);
	}

	void SetRadians(float r) 
	{
		m_radians = r;
		m_degrees = RAD_TO_DEG(r);
	}
};