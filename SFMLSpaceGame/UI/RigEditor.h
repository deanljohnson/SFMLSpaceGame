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

	template<class TRigType>
	void CreateNewRig(const std::string& name);

	void OnRigTypeSelected(const std::string& type);
	void OnRigSelected(const std::string& name);
	void OnSaveRig();
	void OnDeleteRig();
public:
	RigEditor();
};

template <class TRigType>
void RigEditor::CreateNewRig(const std::string& name)
{
	// If user cancelled rig creation
	if (name.empty())
		return;

	TRigType newRig = TRigType();
	newRig.name = name;

	Serializer<> ser;
	ser.Save(&newRig, name, name);

	// if the name already has it's extension appended
	if (StringHelper::StrEndsWith(name, TRigType::GetTypeName()))
	{
		OnRigSelected(name);
	}
	else // need to add the extension here
	{
		OnRigSelected(name + "." + TRigType::GetTypeName());
	}
}