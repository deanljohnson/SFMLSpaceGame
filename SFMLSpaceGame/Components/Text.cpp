#include <Components/Text.h>
#include <UI/UI.h>
#include <UI/UIText.h>
#include <UI/UICenterOn.h>
#include <resource.h>

namespace {
	UI_ID centerID;
}

void Text::Update()
{
	CENTER_ON(UI_ID_NULL, centerID,
		INIT_AND_REFRESH(UIText, m_textID,
			ON_INIT(m_text, FONT_ONE, UITransform(0, 100)))
	);
}
