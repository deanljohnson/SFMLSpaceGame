#include <Components/TextOnSensor.h>
#include <UI/UI.h>
#include <UI/UIText.h>
#include <UI/UICenterOn.h>
#include <resource.h>
#include "EntitySensor.h"

namespace {
	UI_ID centerID;
}

void TextOnSensor::Init()
{
	auto sensor = &entity->GetComponent<EntitySensor>();

	sensor->AddCallback(
		[this](bool state, Sensor* sensor)
		{
			m_display = state;
		});
}

void TextOnSensor::Update()
{
	if (m_display)
		CENTER_ON(UI_ID_NULL, centerID,
			INIT_AND_REFRESH(UIText, m_textID,
				ON_INIT(m_text, FONT_ONE, UITransform(0, 100)))
		);
		
}
