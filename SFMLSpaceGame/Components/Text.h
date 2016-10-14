#pragma once
#include "Component.h"
#include <SFGUI/Label.hpp>

class Text : public Component
{
private:
	sfg::Label::Ptr m_label;
public:
	explicit Text(const std::string& text);
	virtual void OnEnable() override;
	virtual void OnDisable() override;
};