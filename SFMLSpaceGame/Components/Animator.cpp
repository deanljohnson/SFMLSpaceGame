#include "stdafx.h"
#include <Components\Animator.h>
#include <Components\Sprite.h>
#include <ResourceLoader.h>
#include <Entity.h>
#include <GameTime.h>

Animator::Animator(EntityID ent, const std::string& id, ComponentID spriteID)
	: Component(ent),
	  m_sprite(entity->GetComponent<Sprite>(spriteID)),
	  m_animation(LoadAnimationResource(id)),
	  m_id(id)
{
	m_sprite.SetTextureRect(m_animation.GetCurrentFrame());
}

void Animator::Update() 
{
	if (m_animation.Update(GameTime::deltaTime)) 
	{
		m_sprite.SetTextureRect(m_animation.GetCurrentFrame());
	}

	if (next != nullptr) next->Update();
}

float Animator::GetLength()
{
	return m_animation.GetLength();
}

float Animator::GetSpeed()
{
	return m_animation.GetSpeed();
}