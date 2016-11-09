#include "stdafx.h"
#include <Components/MusicSource.h>

MusicSource::MusicSource(EntityID ent, const std::string& filename)
	: Component(ent)
{
	m_music.openFromFile(filename);
	m_music.setLoop(true);
	m_music.setVolume(50.f);
	m_music.play();
}