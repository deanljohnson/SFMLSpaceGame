#include "stdafx.h"
#include <Entity.h>
#include <VectorMath.h>
#include <Components/DirectionalVelocity.h>

DirectionalVelocity::DirectionalVelocity(EntityID ent, float maxVel)
	: Component(ent),
	  m_input(entity->GetComponent<DirectionalKeyboardInput>()),
	  m_physics(entity->GetComponent<Physics>()),
	  m_maxVelocity(maxVel)
	  
{
}

void DirectionalVelocity::Update() 
{
	// Move a vector depending on the keyboard input
	m_physics.SetVelocity(SFMLVecToB2Vec(m_input.input * m_maxVelocity));
}