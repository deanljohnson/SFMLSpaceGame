#pragma once
#include <Box2D/Dynamics/b2World.h>
#include "WorldConstants.h"

/// Keeps the physics simulation running at the proper rate
/// and with a fixed timestep
class PhysicsStepper
{
private:
	float m_accumulator;
	
public:
	void Step(b2World& world, float frameTime)
	{
		m_accumulator += frameTime;

		while (m_accumulator >= FIXED_TIME_STEP)
		{
			world.Step(FIXED_TIME_STEP, BOX2D_VELOCITY_ITERATIONS, BOX2D_POSITION_ITERATIONS);
			m_accumulator -= FIXED_TIME_STEP;
		}
	}
};