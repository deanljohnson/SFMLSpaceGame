#include "ParallaxMovement.h"

void ParallaxMovement::Init()
{
	m_position = &entity->GetComponent<Position>();

	m_targetLastPosition = m_targetPosition->position;
}

void ParallaxMovement::Update()
{
	//how much the target moved last frame
	auto dif = m_targetPosition->position - m_targetLastPosition;
	dif *= m_movementScale;

	m_accumulatedOffset += dif;

	m_position->position = m_targetPosition->position - m_accumulatedOffset;
	
	m_targetLastPosition = m_targetPosition->position;
}

