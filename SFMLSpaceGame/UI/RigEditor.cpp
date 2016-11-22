#include "stdafx.h"
#include <UI/RigEditor.h>
#include <UI/LaserRigEditorWidget.h>
#include <UI/RigTypeSelector.h>
#include <UI/RigNameEntry.h>

RigEditor::RigEditor()
	: GameWindow("rig_editor")
{
	// Create main window
	m_window = sfg::Window::Create(sfg::Window::TOPLEVEL | sfg::Window::CLOSE);
	m_window->SetTitle("Rig Editor");
	m_window->SetId("rig_editor");
	UI::Singleton->Add(m_window);
	SetupWindowSignals();

	// Create Widgets
	m_createRigButton = sfg::Button::Create("Create Rig");
	m_editRigButton = sfg::Button::Create("Edit Rig");
	m_saveRigButton = sfg::Button::Create("Save Rig");
	m_deleteRigButton = sfg::Button::Create("Delete Rig");
	SetupButtonSignals();

	// Create layout widgets
	auto topLevelBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
	auto leftSideBar = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
	m_mainBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);

	leftSideBar->Pack(m_createRigButton);
	leftSideBar->Pack(m_editRigButton);
	leftSideBar->Pack(m_saveRigButton);
	leftSideBar->Pack(m_deleteRigButton);

	topLevelBox->Pack(leftSideBar);
	topLevelBox->Pack(m_mainBox);
	m_window->Add(topLevelBox);
}

void RigEditor::SetupButtonSignals()
{
	m_createRigButton->GetSignal(sfg::Button::OnLeftClick).Connect(
		[this]
		{
			auto selectWindow = GetWindow<RigTypeSelector>("rig_type_selector");
			selectWindow->SetCallback([this](const std::string& name) { OnRigTypeSelected(name); });
			selectWindow->Show(true);
		});

	m_editRigButton->GetSignal(sfg::Button::OnLeftClick).Connect(
		[this]
		{
			auto selectWindow = GetWindow<RigSelector>("rig_select");
			selectWindow->SetCallback([this](const std::string& name) { OnRigSelected(name); });
			selectWindow->Show(true);
		});

	m_saveRigButton->GetSignal(sfg::Button::OnLeftClick).Connect(
		[this]
		{
			OnSaveRig();
		});

	m_deleteRigButton->GetSignal(sfg::Button::OnLeftClick).Connect(
		[this]
		{
			if (m_rigWidget.get() == nullptr)
				return;
			auto editWindow = GetWindow<ConfirmationDialog>("confirmation_dialog");
			editWindow->SetCallback([this](bool val) { if (val) OnDeleteRig(); });
			editWindow->SetText("Are you sure you want to delete this rig? This cannot be undone.");
			editWindow->CenterOnScreen(); // changing the text can make the window uncentered
			editWindow->Show(true);
		});
}

void RigEditor::OnRigTypeSelected(const std::string& type)
{
	if (type.empty())
		return;

	auto rigNameEntry = GetWindow<RigNameEntry>("rig_name_entry");
	rigNameEntry->Show(true);

	if (type == "Laser")
	{
		rigNameEntry->SetCallback([this](const std::string& name) { CreateNewRig<LaserRig>(name); });
	}
	else
	{
		throw "unrecognized rig type '" + type + "'";
	}
}

void RigEditor::OnRigSelected(const std::string& name)
{
	// If user canceled rig selection
	if (name.size() == 0)
		return;

	// If there was a previous rig widget
	if (m_rigWidget != nullptr)
		m_mainBox->Remove(m_rigWidget);

	if (StringHelper::StrEndsWith(name, LaserRig::GetTypeName()))
	{
		auto noExt = name;
		noExt.erase(noExt.find_last_of("."), std::string::npos);

		m_rigEditWidget = std::make_shared<LaserRigEditorWidget>(noExt);
		m_rigWidget = m_rigEditWidget->GetWidget();
		m_mainBox->PackStart(m_rigWidget);
	}
}

void RigEditor::OnSaveRig()
{
	if (m_rigEditWidget != nullptr)
		m_rigEditWidget->Save();
}

void RigEditor::OnDeleteRig()
{
	if (m_rigEditWidget != nullptr)
		m_rigEditWidget->Delete();
}