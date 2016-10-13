#include <UI/ShipEditorWindow.h>
#include <UI/UI.h>
#include <UI/UIExpandingBackground.h>
#include <resource.h>

ShipEditorWindow::ShipEditorWindow() 
{

}

void ShipEditorWindow::Update() 
{
	MAKE_HIERARCHY(
		INIT_AND_REFRESH(UIExpandingBackground, m_ids[0],
			ON_INIT(UI_BACKGROUND_METAL_NINE, sf::Vector2i(500, 400))),
		INIT_AND_REFRESH(UIExpandingBackground, m_ids[1],
			ON_INIT(UI_BACKGROUND_METAL_NINE, sf::Vector2i(100, 100), UITransform(50, 50))));
	
}