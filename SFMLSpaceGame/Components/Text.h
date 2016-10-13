#pragma once
#include "Component.h"
#include <UI/UI_ID.h>

class Text : public Component
{
private:
	std::string m_text;
	UI_ID m_textID{ UI_ID_NULL };

public:
	explicit Text(const std::string& text)
		: m_text(text)
	{}

	virtual void Update() override;
};