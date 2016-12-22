// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include <Components/Component.h>
#include <EntityManager.h>

Component::Component()
	: next(nullptr)
{}

Component::Component(EntityID ent)
	: next(nullptr), entity(EntityManager::Get(ent))
{}

EntityID Component::GetEntityID() const
{
	return entity.GetID();
}