#pragma once
#include "Component.h"
#include <SFGUI/Label.hpp>
#include <cereal/access.hpp>

class Text : public Component
{
private:
	sfg::Label::Ptr m_label;

	friend class cereal::access;

	// used for saving
	template <class Archive>
	void serialize(Archive& ar)
	{
		ar(entity.GetID(), m_label->GetText().toAnsiString());
	}

	template <class Archive>
	static void load_and_construct(Archive& ar, cereal::construct<Text>& construct)
	{
		EntityID selfID;
		std::string text;
		ar(selfID, text);
		construct(selfID, text);
	}

public:
	Text(EntityID ent, const std::string& text);
	virtual void OnEnable() override;
	virtual void OnDisable() override;
};