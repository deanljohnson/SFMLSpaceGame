// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include <ComponentSerializer.h>
#include <Entity.h>
#include <Components\Components.h>

#ifndef ALL_COMPONENTS
#define ALL_COMPONENTS Position, \
Rotation, \
Sprite, \
Animator,\
Shields,\
ShieldHitAnimator,\
Physics,\
SmoothCameraFollow,\
ParallaxMovement,\
DirectionalKeyboardInput,\
ShipThrusters,\
ThrusterInput,\
RectPrimitive,\
RotateToFaceMouse,\
GameWorldClickListener,\
SoundSource,\
SoundListener,\
MusicSource,\
DirectionalGun,\
MissileLauncher,\
MiningLaser,\
FireGunOnClick<DirectionalGun>,\
FireGunOnClick<MissileLauncher>,\
FireGunOnClick<MiningLaser>,\
BulletPhysics,\
MissilePhysics,\
MissileController,\
Lifetime,\
CollisionFilterComponent,\
ShipController,\
EntitySensor,\
ZoomHandler,\
ShipAI,\
Health,\
DamageOnAttacked,\
PlayerDeathBroadcaster,\
ThrusterAnimator,\
GunHeatUIDisplay<DirectionalGun>,\
TilingBackground,\
KeyListener,\
Text,\
Inventory,\
ItemPickup,\
EconomyAgent,\
ParallaxTargetAssigner,\
ShipStatsComponent,\
OreVein
#endif

void ComponentSerializer::Load(ComponentID id,
							   cereal::JSONInputArchive& ar, 
								Entity& ent) 
{
	LoadComponents<cereal::JSONInputArchive, ALL_COMPONENTS>(id, ar, ent);
}

void ComponentSerializer::Load(ComponentID id,
								cereal::BinaryInputArchive& ar,
								Entity& ent)
{
	LoadComponents<cereal::BinaryInputArchive, ALL_COMPONENTS>(id, ar, ent);
}

void ComponentSerializer::Save(ComponentID id,
								cereal::JSONOutputArchive& ar,
								const Entity& ent)
{
	SaveComponents<cereal::JSONOutputArchive, ALL_COMPONENTS>(id, ar, ent);
}

void ComponentSerializer::Save(ComponentID id,
								cereal::BinaryOutputArchive& ar,
								const Entity& ent)
{
	SaveComponents<cereal::BinaryOutputArchive, ALL_COMPONENTS>(id, ar, ent);
}

/*void ComponentSerializer::Serialize(SERIALIZATION_IN_ARCHIVE& ar, Entity& ent)
{
	// Need to figure out the looping mechanism for this
	ComponentID id;
	ar(id);

	while (id != std::numeric_limits<ComponentID>::max()) 
	{
		Load(id, ar, ent);

		ar(id); // get the next component ID
	}
}

void ComponentSerializer::Serialize(SERIALIZATION_OUT_ARCHIVE& ar, const Entity& ent)
{
	for (int i = 0; i < maxComponents; i++) 
	{
		if (ent.m_componentBitset[i]) 
		{
			Save(i, ar, ent);
		}
	}

	// ComponentID::max is used as a flag to indicate the end of 
	// component section in the serialized representation
	ar(std::numeric_limits<ComponentID>::max());
}*/