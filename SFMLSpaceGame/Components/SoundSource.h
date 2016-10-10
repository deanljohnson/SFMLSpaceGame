#pragma once
#include "Position.h"
#include <memory>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/Sound.hpp>
#include <ResourceLoader.h>

class SoundSource : public Component
{
private:
	Position* m_position{ nullptr };
	std::shared_ptr<sf::SoundBuffer> m_soundBuffer{ nullptr };
	sf::Sound m_sound;
	ResourceID m_soundID;
public:
	explicit SoundSource(ResourceID soundID)
		: m_soundID(soundID)
	{}
	virtual void Init() override;
	void Play();
};