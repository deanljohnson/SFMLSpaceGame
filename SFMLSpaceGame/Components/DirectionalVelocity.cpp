#include <Entity.h>
#include <VectorMath.h>
#include <Components/DirectionalVelocity.h>

void DirectionalVelocity::Init()
{
	m_physics = &entity->GetComponent<Physics>();
	m_input = &entity->GetComponent<DirectionalKeyboardInput>();
}

void DirectionalVelocity::Update() 
{
	m_physics->SetVelocity(SFMLVecToB2Vec(m_input->input * m_maxVelocity));
}