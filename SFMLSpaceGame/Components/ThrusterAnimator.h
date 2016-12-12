#pragma once
#include <cereal\access.hpp>
#include <cereal\types\vector.hpp>

class ShipThrusters;

class ThrusterAnimator : public Component, public Updateable
{
private:
	ShipThrusters& m_thrusters;
	std::vector<Sprite*> m_thrusterSprites;

	float m_currentScale;

	void ApplyScale();

	friend class cereal::access;

	// used for saving
	template <class Archive>
	void serialize(Archive& ar)
	{
		// determine the IDs of sprites being animated
		std::vector<int> spriteIDs;
		for (auto* spr : m_thrusterSprites) 
		{
			spriteIDs.push_back(entity->GetComponentID<Sprite>(*spr));
		}

		ar(entity.GetID(), m_currentScale, spriteIDs);
	}

	template <class Archive>
	static void load_and_construct(Archive& ar, cereal::construct<ThrusterAnimator>& construct)
	{
		EntityID selfID;
		ar(selfID);
		construct(selfID);

		std::vector<int> spriteIDs;
		ar(construct->m_currentScale, spriteIDs);

		// Get the saved sprites
		for (auto i : spriteIDs) 
		{
			construct->AddSprite(&construct->entity->GetComponent<Sprite>(i));
		}
	}
public:
	explicit ThrusterAnimator(EntityID ent);

	virtual void Update() override;

	void AddSprite(Sprite* sprite);

	std::vector<Sprite*>& GetSprites();
};