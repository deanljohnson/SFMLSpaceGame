#pragma once
class Position;

class SmoothCameraFollow : public Component
{
private:
	Position& m_position;
	b2Vec2 m_lastPosition;

	friend class cereal::access;

	// used for saving
	template <class Archive>
	void serialize(Archive& ar)
	{
		ar(entity.GetID(), m_lastPosition);
	}

	template <class Archive>
	static void load_and_construct(Archive& ar, cereal::construct<SmoothCameraFollow>& construct)
	{
		EntityID selfID;
		ar(selfID);
		construct(selfID);

		ar(construct->m_lastPosition);
	}

public:
	explicit SmoothCameraFollow(EntityID ent);
	virtual void Update() override;
};