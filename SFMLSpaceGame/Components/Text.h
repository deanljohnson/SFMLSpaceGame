#pragma once
#include "Component.h"
#include <SFGUI/Label.hpp>
#include <SFGUI/Alignment.hpp>

class Text : public Component
{
private:
	sfg::Label::Ptr m_label;
	sfg::Alignment::Ptr m_centerAlignment;
public:
	explicit Text(const std::string& text);
	virtual void OnEnable() override;
	virtual void OnDisable() override;
};