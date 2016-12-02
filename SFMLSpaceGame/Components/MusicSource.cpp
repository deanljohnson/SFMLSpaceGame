// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include <Components/MusicSource.h>

MusicSource::MusicSource(EntityID ent, const std::string& filename)
	: Component(ent),
	  m_musicFilename(filename)
{
	m_music.openFromFile(filename);
	m_music.setLoop(true);
	m_music.setVolume(50.f);
	m_music.play();
}