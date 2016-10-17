#include <Components/ParallaxMovement.h>
#include <EntityManager.h>

void ParallaxMovement::Init()
{
	m_targetHandle = EntityManager::Get(m_targetID);
	m_targetPosition = &m_targetHandle->GetComponent<Position>();

	m_position = &entity->GetComponent<Position>();
	m_targetLastPosition = m_targetPosition->position;
}

void ParallaxMovement::Update()
{
	if (!m_targetHandle.IsValid())
		return;
	//how much the target moved last frame
	auto dif = m_targetPosition->position - m_targetLastPosition;
	dif *= m_movementScale;

	m_position->position += dif;

	m_targetLastPosition = m_targetPosition->position;
}

void ParallaxMovement::SetTarget(const EntityHandle& targetHandle)
{
	m_targetHandle = targetHandle;
	m_targetPosition = &m_targetHandle->GetComponent<Position>();
}

bool ParallaxMovement::HasValidTarget()
{
	return m_targetHandle.IsValid();
}

