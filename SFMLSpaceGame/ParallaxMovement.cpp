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

	m_position->position += dif;
	printf("x: %.2f y: %.2f\n", m_targetPosition->X(), m_targetPosition->Y());
	m_targetLastPosition = m_targetPosition->position;
}

