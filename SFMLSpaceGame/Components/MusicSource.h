#pragma once
#include "Component.h"
#include <SFML/Audio/Music.hpp>

class MusicSource : public Component
{
private:
	sf::Music m_music;
public:
	MusicSource(EntityID ent, const std::string& filename);
};