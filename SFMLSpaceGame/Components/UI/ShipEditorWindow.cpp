#include <UI/UI.h>
#include <UI/UIExpandingBackground.h>
#include <UI/UICenterOn.h>
#include <UI/UIButton.h>
#include <UI/UIText.h>
#include <resource.h>
#include "ShipEditorWindow.h"

ShipEditorWindow::ShipEditorWindow() 
{

}

void ShipEditorWindow::Update() 
{
	auto onExit = [this](UIButton* button) { SetActive(false); };

	CENTER_ON(UI_ID_NULL, m_ids[0],
		MAKE_HIERARCHY(
			INIT_AND_REFRESH(UIExpandingBackground, m_ids[1],
				ON_INIT(UI_BACKGROUND_METAL_NINE, sf::Vector2i(500, 400))),
			CENTER_ON(
				INIT_AND_REFRESH(UIButton, m_ids[2],
					ON_INIT(UI_BUTTON_TEST_ONE, onExit, UITransform(30, 30))), m_ids[3],
				INIT_AND_REFRESH(UIText, m_ids[4],
					ON_INIT(UITextOption{"Click to Exit!", FONT_ONE, sf::Color::Black, 12}))
			)
		)
	);

	
}