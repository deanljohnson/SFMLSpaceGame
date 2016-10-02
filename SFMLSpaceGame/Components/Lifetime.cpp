#include <Components/Lifetime.h>
#include <GameTime.h>
#include <Entity.h>

void Lifetime::Update()
{
	m_lifetime -= GameTime::deltaTime;

	if (m_lifetime <= 0)
		entity->Destroy();
}