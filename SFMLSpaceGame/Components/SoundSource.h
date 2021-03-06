#pragma once
#include <memory>
#include <SFML/Audio/Sound.hpp>

namespace sf {
	class SoundBuffer;
}
class Position;

class SoundSource : public Component
{
private:
	Position& m_position;
	std::shared_ptr<sf::SoundBuffer> m_soundBuffer;
	sf::Sound m_sound;
	int m_soundID;

	friend class cereal::access;

	// used for saving
	template <class Archive>
	void serialize(Archive& ar)
	{
		ar(entity.GetID(), m_soundID);
	}

	template <class Archive>
	static void load_and_construct(Archive& ar, cereal::construct<SoundSource>& construct)
	{
		EntityID selfID;
		int soundID;
		ar(selfID, soundID);
		construct(selfID, soundID);
	}
public:
	SoundSource(EntityID ent, int soundID);
	void Play();
};