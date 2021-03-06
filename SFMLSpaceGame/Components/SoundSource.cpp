// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include <Components/SoundSource.h>
#include <Components/Position.h>
#include <Entity.h>

SoundSource::SoundSource(EntityID ent, int soundID)
	: Component(ent),
	  m_position(entity->GetComponent<Position>()),
	  m_soundBuffer(LoadSoundBuffer(soundID)),
	  m_soundID(soundID)
{
	if (m_soundBuffer == nullptr)
	{
		printf("The given sound ID was invalid: %d\n", m_soundID);
		return;
	}

	m_sound.setBuffer(*m_soundBuffer.get());
	m_sound.setMinDistance(.2f);
	m_sound.setAttenuation(.1f);
}

void SoundSource::Play()
{
	if (m_soundBuffer == nullptr)
		return;
	// Sound is spatialized in 3D by SFML
	// The y axis corresponds to the third dimension in
	// SFML's coordinates
	m_sound.setPosition(m_position.X(), 0.f, m_position.Y());
	m_sound.play();
}