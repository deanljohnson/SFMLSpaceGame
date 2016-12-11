// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include <UI/ImageSelector.h>
#include <UI/UI.h>
#include <FileSystem.h>
#include <TextureMap.h>

ImageSelector::ImageSelector()
	: GameWindow("image_select"),
	m_checkForAtlas(true),
	m_selectedHasAtlas(false)
{
	// Create main window
	m_window = sfg::Window::Create(sfg::Window::TOPLEVEL | sfg::Window::CLOSE);
	m_window->SetTitle("Select Image");
	m_window->SetId("image_select");
	UI::Singleton->Add(m_window);
	SetupWindowSignals();

	auto table = sfg::Table::Create();
	m_window->Add(table);

	m_imageSelector = sfg::ComboBox::Create();
	table->Attach(m_imageSelector, { 0, 0, 2, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	m_atlasKeySelector = sfg::ComboBox::Create();
	table->Attach(m_atlasKeySelector, { 0, 1, 2, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	m_atlasKeySelector->Show(false);

	std::vector<std::string> shipFiles;
	FileSystem::GetFileNamesWithExtension(IMAGE_PATH, ".png", shipFiles);
	for (auto& e : shipFiles)
	{
		// strip the .png extension
		e = e.substr(0, e.size() - 4);

		// Add it to the selection box
		m_imageSelector->AppendItem(e);
	}

	auto selectButton = sfg::Button::Create("Select");
	auto cancelButton = sfg::Button::Create("Cancel");
	table->Attach(selectButton, { 0, 2, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	table->Attach(cancelButton, { 1, 2, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);

	m_imageSelector->GetSignal(sfg::ComboBox::OnSelect).Connect(
		[this] { OnSelectionChange(); });
	selectButton->GetSignal(sfg::Button::OnLeftClick).Connect(
		[this] { OnSelectClicked(); });
	cancelButton->GetSignal(sfg::Button::OnLeftClick).Connect(
		[this] { OnCancel(); });
}

void ImageSelector::SetCheckForAtlas(bool val)
{
	m_checkForAtlas = val;
}

void ImageSelector::SetCallback(std::function<void(const SpriteKey&)> callback)
{
	m_callback = callback;
}

void ImageSelector::OnSelectionChange()
{
	const auto& name = m_imageSelector->GetSelectedText();

	if (!m_checkForAtlas)
		m_atlasKeySelector->Show(false);
	else
	{
		m_atlasKeySelector->Clear();
		// Is the selected file an atlas
		if (FileSystem::DoesFileExist(DATA_PATH + name + "." + TextureMap<std::string>::GetTypeName()))
		{
			auto atlas = LoadTextureMap<std::string>(name);
			for (auto& kvp : atlas->GetMap())
			{
				m_atlasKeySelector->AppendItem(kvp.first);
			}

			m_selectedHasAtlas = true;
			m_atlasKeySelector->Show(true);
		}
		else m_selectedHasAtlas = false;
	}
}

void ImageSelector::OnSelectClicked()
{
	m_atlasKeySelector->Show(false);

	if (m_checkForAtlas 
		&& m_selectedHasAtlas 
		&& !m_atlasKeySelector->GetSelectedText().isEmpty())
		CallCallback({ m_imageSelector->GetSelectedText(), m_atlasKeySelector->GetSelectedText() });
	else
		CallCallback({ m_imageSelector->GetSelectedText() });

	Show(false);
}

void ImageSelector::OnCancel()
{
	CallCallback({}); 
	Show(false);
}

void ImageSelector::CallCallback(const SpriteKey& key)
{
	if (m_callback != nullptr)
		m_callback(key);
}