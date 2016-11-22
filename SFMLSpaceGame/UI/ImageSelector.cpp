#include "stdafx.h"
#include <UI/ImageSelector.h>
#include "UI.h"
#include <FileSystem.h>

ImageSelector::ImageSelector()
	: GameWindow("image_select")
{
	// Create main window
	m_window = sfg::Window::Create(sfg::Window::TOPLEVEL | sfg::Window::CLOSE);
	m_window->SetTitle("Select Image");
	m_window->SetId("image_select");
	UI::Singleton->Add(m_window);
	SetupWindowSignals();

	auto table = sfg::Table::Create();
	m_window->Add(table);

	auto selectionBox = sfg::ComboBox::Create();
	table->Attach(selectionBox, { 0, 0, 2, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);

	std::vector<std::string> shipFiles;
	FileSystem::GetFileNamesWithExtension(IMAGE_PATH, ".png", shipFiles);
	for (auto e : shipFiles)
	{
		// TODO: Strip the extension from the file name
		selectionBox->AppendItem(e);
	}

	auto selectButton = sfg::Button::Create("Select");
	auto cancelButton = sfg::Button::Create("Cancel");
	table->Attach(selectButton, { 0, 1, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	table->Attach(cancelButton, { 1, 1, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);

	selectButton->GetSignal(sfg::Button::OnLeftClick).Connect(
		[this, selectionBox] { CallCallback(selectionBox->GetSelectedText()); Show(false); });
	cancelButton->GetSignal(sfg::Button::OnLeftClick).Connect(
		[this] { CallCallback(""); Show(false); });
}

void ImageSelector::SetCallback(std::function<void(const std::string&)> callback)
{
	m_callback = callback;
}

void ImageSelector::CallCallback(const std::string& name)
{
	if (m_callback != nullptr)
		m_callback(name);
}