// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include <Components/MiningLaser.h>
#include <Components/Position.h>
#include <Components/Rotation.h>
#include <Components/SoundSource.h>
#include <Components/OreVein.h>
#include <Components/Inventory.h>
#include <Entity.h>
#include <MiningLaserTargeter.h>
#include <BatchIndex.h>
#include <RenderBatch.h>
#include <GameTime.h>
#include <EntityManager.h>
#include <ItemFactory.h>

namespace {
	MiningLaserTargeter targeter{2.5f};
}

MiningLaser::MiningLaser(EntityID ent)
	: Component(ent),
	m_position(entity->GetComponent<Position>()),
	m_rotation(entity->GetComponent<Rotation>()),
	m_laserData(nullptr),
	m_batchIndex(nullptr),
	m_laserTexAnimOffset(0.f),
	m_laserTexAnimRate(-120.f),
	m_firing(false),
	m_targetID(ENTITY_ID_NULL)
{
}

void MiningLaser::Update()
{
	if (m_laserData == nullptr)
		return;

	m_laserTexAnimOffset += m_laserTexAnimRate * GameTime::deltaTime;
	m_laserTexAnimOffset = fmodf(m_laserTexAnimOffset, static_cast<float>(m_batchIndex->GetTexture()->getSize().x));

	if (!m_firing)
	{
		HideLaser();
	}
	else
	{
		ShowLaser();
		m_firing = false;

		if (m_targetID != ENTITY_ID_NULL)
		{
			DoMining();
		}
	}
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

void MiningLaser::HideLaser()
{
	m_batchIndex->SetColor(sf::Color::Transparent);
}

void MiningLaser::ShowLaser()
{
	m_batchIndex->SetColor(sf::Color::White);
}

void MiningLaser::DoMining()
{
	if (!m_targetHandle.IsValid())
	{
		m_targetID = ENTITY_ID_NULL;
		return;
	}

	m_miningTimer -= GameTime::deltaTime;
	if (m_miningTimer < 0.f)
	{
		m_miningTimer = m_laserData->rig->miningSpeed;

		auto& targetOre = m_targetHandle->GetComponent<OreVein>();
		auto minedAmount = targetOre.Mine(m_laserData->rig->miningAmount);

		if (minedAmount != 0)
		{
			auto& inven = entity->GetComponent<Inventory>();
			inven.AddItem(ItemFactory::Create<ItemType::Ore>(minedAmount));
		}
	}
}

void MiningLaser::Shoot(const b2Vec2& pos)
{
	if (m_laserData == nullptr)
	{
#ifdef _DEBUG
		printf("Cannot fire a mining laser without first given it a MiningLaserData pointer\n");
#endif
		return;
	}

	ShowLaser();
	m_firing = true;

	auto startingID = m_targetID;
	auto targetPos = targeter.FindTarget(pos, GetEntityID(), m_targetID);

	if (startingID != m_targetID
		&& m_targetID != ENTITY_ID_NULL)
	{
		m_targetHandle = EntityManager::Get(m_targetID);
		m_miningTimer = m_laserData->rig->miningSpeed;
	}

	// Distance to target
	auto dist = b2Distance(targetPos, m_position.position);

	auto halfLaserThickness = m_miningLaserThickness / 2.f;

	// We will determine vertex positions in local coordinates first
	auto vert1Pos = b2Vec2(0, -halfLaserThickness);
	auto vert2Pos = b2Vec2(dist, -halfLaserThickness);
	auto vert3Pos = b2Vec2(dist, halfLaserThickness);
	auto vert4Pos = b2Vec2(0, halfLaserThickness);

	// Apply current rotation
	auto dif = targetPos - m_position.position;
	auto radians = atan2f(dif.y, dif.x);
	//auto radians = m_rotation.GetRadians();
	vert1Pos = Rotate(vert1Pos, radians);
	vert2Pos = Rotate(vert2Pos, radians);
	vert3Pos = Rotate(vert3Pos, radians);
	vert4Pos = Rotate(vert4Pos, radians);

	// Apply current position
	vert1Pos += m_position.position;
	vert2Pos += m_position.position;
	vert3Pos += m_position.position;
	vert4Pos += m_position.position;

	(*m_batchIndex)[0].position = B2VecToSFMLVec(vert1Pos);
	(*m_batchIndex)[1].position = B2VecToSFMLVec(vert2Pos);
	(*m_batchIndex)[2].position = B2VecToSFMLVec(vert3Pos);
	(*m_batchIndex)[3].position = B2VecToSFMLVec(vert4Pos);

	auto texLength = m_batchIndex->GetTexture()->getSize().x;
	auto endTexCoord = dist * texLength;
	
	(*m_batchIndex)[0].texCoords.x = m_laserTexAnimOffset;
	(*m_batchIndex)[1].texCoords.x = endTexCoord + m_laserTexAnimOffset;
	(*m_batchIndex)[2].texCoords.x = endTexCoord + m_laserTexAnimOffset;
	(*m_batchIndex)[3].texCoords.x = m_laserTexAnimOffset;
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
#ifdef _DEBUG
	if (m_laserData != nullptr)
	{
		printf("Re-assignment of MiningLaserData is most likely an error\n");
	}
#endif

	m_laserData = data;

	m_batchIndex = RenderBatch::Get(m_laserData->rig->imageLocation)->Add();
	m_miningLaserThickness = m_batchIndex->GetRect().height * METERS_PER_PIXEL;
	m_batchIndex->SetColor(sf::Color::Transparent);
	m_batchIndex->SetOrigin({ 0, m_miningLaserThickness / 2.f * METERS_PER_PIXEL });
	m_batchIndex->GetTexture()->setRepeated(true);
}