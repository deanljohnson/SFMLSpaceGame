#include "stdafx.h"
#include <ComponentSerializer.h>
#include <Entity.h>
#include <Components\Components.h>

void ComponentSerializer::Load(ComponentID id, 
								SERIALIZATION_IN_ARCHIVE& ar, 
								Entity& ent) 
{
	LoadComponents<Position, 
		Rotation, 
		Sprite, 
		Animator,
		Shields,
		ShieldHitAnimator,
		Physics,
		SmoothCameraFollow,
		ParallaxMovement,
		DirectionalKeyboardInput,
		ShipThrusters,
		ThrusterInput,
		RectPrimitive,
		RotateToFaceMouse,
		GameWorldClickListener,
		DirectionalGun,
		FireGunOnClick,
		BulletPhysics,
		Lifetime,
		CollisionFilterComponent,
		ShipController,
		EntitySensor,
		ZoomHandler,
		ShipAI,
		Health,
		DamageOnAttacked,
		//ShipSpawner,
		PlayerDeathBroadcaster,
		ThrusterAnimator,
		SoundSource,
		SoundListener,
		MusicSource,
		GunHeatUIDisplay<DirectionalGun>,
		TilingBackground,
		KeyListener,
		Text,
		Inventory,
		ItemPickup, 
		EconomyAgent,
		ParallaxTargetAssigner,
		ShipStatsComponent
	>(id, ar, ent);
}

void ComponentSerializer::Save(ComponentID id,
								SERIALIZATION_OUT_ARCHIVE& ar,
								const Entity& ent)
{
	SaveComponents<Position, 
		Rotation,
		Sprite,
		Animator,
		Shields,
		ShieldHitAnimator,
		Physics,
		SmoothCameraFollow,
		ParallaxMovement,
		DirectionalKeyboardInput,
		ShipThrusters,
		ThrusterInput,
		RectPrimitive,
		RotateToFaceMouse,
		GameWorldClickListener,
		DirectionalGun,
		FireGunOnClick,
		BulletPhysics,
		Lifetime,
		CollisionFilterComponent,
		ShipController,
		EntitySensor,
		ZoomHandler,
		ShipAI,
		Health,
		DamageOnAttacked,
		//ShipSpawner,
		PlayerDeathBroadcaster,
		ThrusterAnimator,
		SoundSource,
		SoundListener,
		MusicSource,
		GunHeatUIDisplay<DirectionalGun>,
		TilingBackground,
		KeyListener,
		Text,
		Inventory,
		ItemPickup,
		EconomyAgent,
		ParallaxTargetAssigner,
		ShipStatsComponent
	>(id, ar, ent);
}

void ComponentSerializer::Serialize(SERIALIZATION_IN_ARCHIVE& ar, Entity& ent)
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
}