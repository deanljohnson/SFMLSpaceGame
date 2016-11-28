#pragma once
#include <SFML/Audio/Music.hpp>
#include <cereal/access.hpp>

class MusicSource : public Component
{
private:
	sf::Music m_music;
	std::string m_musicFilename;

	friend class cereal::access;

	// used for saving
	template <class Archive>
	void serialize(Archive& ar)
	{
		ar(entity.GetID(), m_musicFilename);
	}

	template <class Archive>
	static void load_and_construct(Archive& ar, cereal::construct<MusicSource>& construct)
	{
		EntityID selfID;
		std::string filename;
		ar(selfID, filename);
		construct(selfID, filename);
	}
public:
	MusicSource(EntityID ent, const std::string& filename);
};