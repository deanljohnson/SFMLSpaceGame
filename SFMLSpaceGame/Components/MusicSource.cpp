#include "stdafx.h"
#include <Components/MusicSource.h>

void MusicSource::Init()
{
	m_music.openFromFile(m_filename);
	m_music.setLoop(true);
	m_music.setVolume(50.f);
	m_music.play();
}