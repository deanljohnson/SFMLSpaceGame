#pragma once
#include "Component.h"
#include <SFML/Audio/Music.hpp>

class MusicSource : public Component
{
private:
	sf::Music m_music;
	std::string m_filename;
public:
	explicit MusicSource(const std::string& filename)
		: m_filename(filename)
	{}
	virtual void Init() override;
};