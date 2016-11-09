#pragma once
#include "Position.h"
#include <memory>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/Sound.hpp>
#include <ResourceLoader.h>

class SoundSource : public Component
{
private:
	Position& m_position;
	std::shared_ptr<sf::SoundBuffer> m_soundBuffer;
	sf::Sound m_sound;
public:
	SoundSource(EntityID ent, ResourceID soundID);
	void Play();
};