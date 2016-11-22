#pragma once
#include <UI/GameWindow.h>
#include <UI/RigEditorWidget.h>
#include <SFGUI/Widgets.hpp>

class RigEditor : public GameWindow
{
private:
	sfg::Button::Ptr m_createRigButton;
	sfg::Button::Ptr m_editRigButton;
	sfg::Button::Ptr m_saveRigButton;
	sfg::Button::Ptr m_deleteRigButton;

	sfg::Box::Ptr m_mainBox;

	std::shared_ptr<RigEditorWidget> m_rigEditWidget;
	sfg::Widget::Ptr m_rigWidget;

	void SetupButtonSignals();

	void OnRigSelected(const std::string& name);
	void OnSaveRig();
	void OnDeleteRig();
public:
	RigEditor();
};
