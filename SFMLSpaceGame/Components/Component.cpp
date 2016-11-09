#include "stdafx.h"
#include <Components/Component.h>
#include <EntityManager.h>

Component::Component()
	: next(nullptr)
{}

Component::Component(EntityID ent)
	: next(nullptr), entity(EntityManager::Get(ent))
{}