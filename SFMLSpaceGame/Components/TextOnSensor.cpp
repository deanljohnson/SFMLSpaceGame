#include <Components/TextOnSensor.h>
#include <UI/UI.h>
#include <UI/UIText.h>
#include <UI/UICenterOn.h>
#include <resource.h>

namespace {
	UI_ID centerID;
}

void TextOnSensor::Init()
{
	m_sensor = &entity->GetComponent<EntitySensor>();
	//INIT_AND_DISPLAY(UIText, m_textID, m_text, FONT_ONE, UITransform(0, 100));
}

void TextOnSensor::Update()
{
	if (!m_sensor->sensedEntities.empty())
		CENTER_ON(UI_ID_NULL, centerID,
			INIT_AND_DISPLAY(UIText, m_textID, m_text, FONT_ONE, UITransform(0, 100))
		);
		
}
