// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include <Components/MiningLaser.h>
#include <Components/Position.h>
#include <Components/Rotation.h>
#include <Components/SoundSource.h>
#include <Entity.h>
#include <MiningLaserTargeter.h>

namespace {
	MiningLaserTargeter targeter{};
}

MiningLaser::MiningLaser(EntityID ent)
	: Component(ent),
	m_position(entity->GetComponent<Position>()),
	m_rotation(entity->GetComponent<Rotation>()),
	m_laserData(nullptr)
{
}

int MiningLaser::GetSoundSourceID()
{
	return (m_shotSound == nullptr)
		? -1
		: entity->GetComponentID(*m_shotSound);
}

void MiningLaser::InitSoundSource(int compID)
{
	if (compID != -1)
		m_shotSound = &entity->GetComponent<SoundSource>(compID);
}

void MiningLaser::Shoot(const b2Vec2& pos)
{
	auto targetPos = targeter.FindTarget(pos, GetEntityID());
}

float MiningLaser::GetNormalizedHeat()
{
	return 0.f;
}

void MiningLaser::SetSoundSource(SoundSource* source)
{
	m_shotSound = source;
}

void MiningLaser::SetLaserData(MiningLaserData* data)
{
	m_laserData = data;
}