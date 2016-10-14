#pragma once
#include <UI\UI_ID.h>
#include <Components/Component.h>

class ShipEditorWindow : public Component
{
private:
	UI_ID m_ids[10];
public:
	ShipEditorWindow();
	void Update();
};