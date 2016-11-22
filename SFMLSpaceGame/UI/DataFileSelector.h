#pragma once
#include <UI/GameWindow.h>
#include <UI/UI.h>
#include <SFGUI/Widgets.hpp>
#include <FileSystem.h>

template<typename... TDataType>
class DataFileSelector : public GameWindow
{
private:
	std::vector<std::string> m_fileStrings;

	sfg::ComboBox::Ptr m_selectionBox;

	std::function<void(const std::string&)> m_callback;

	void CallCallback(const std::string& name);
	void LoadStrings();
public:
	DataFileSelector(const std::string& key, const std::string& title);

	void SetCallback(std::function<void(const std::string&)> callback);
	virtual void Show(bool val = true) override;
};

template <typename... TDataType>
DataFileSelector<TDataType...>::DataFileSelector(const std::string& key, const std::string& title)
	: GameWindow(key)
{
	// Create main window
	m_window = sfg::Window::Create(sfg::Window::TOPLEVEL | sfg::Window::CLOSE);
	m_window->SetTitle(title);
	m_window->SetId(key);
	UI::Singleton->Add(m_window);
	SetupWindowSignals();

	auto table = sfg::Table::Create();
	m_window->Add(table);

	m_selectionBox = sfg::ComboBox::Create();
	table->Attach(m_selectionBox, { 0, 0, 2, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);

	LoadStrings();

	auto selectButton = sfg::Button::Create("Select");
	auto cancelButton = sfg::Button::Create("Cancel");
	table->Attach(selectButton, { 0, 1, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	table->Attach(cancelButton, { 1, 1, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);

	selectButton->GetSignal(sfg::Button::OnLeftClick).Connect(
		[this] { CallCallback(m_fileStrings[m_selectionBox->GetSelectedItem()]); Show(false); });
	cancelButton->GetSignal(sfg::Button::OnLeftClick).Connect(
		[this] { CallCallback(""); Show(false); });
}

template <typename... TDataType>
void DataFileSelector<TDataType...>::CallCallback(const std::string& name)
{
	if (m_callback != nullptr)
		m_callback(name);
}

template <typename... TDataType>
void DataFileSelector<TDataType...>::LoadStrings()
{
	m_selectionBox->Clear();
	m_fileStrings.clear();
	FileSystem::GetFileNamesWithExtensions(DATA_PATH, { ("." + TDataType::GetTypeName())... }, m_fileStrings);
	for (auto e : m_fileStrings)
	{
		// Strip the extension from the file name
		// for display
		e.erase(e.find_last_of("."), std::string::npos);
		m_selectionBox->AppendItem(e);
	}
}

template <typename... TDataType>
void DataFileSelector<TDataType...>::SetCallback(std::function<void(const std::string&)> callback)
{
	m_callback = callback;
}

template <typename... TDataType>
void DataFileSelector<TDataType...>::Show(bool val)
{
	GameWindow::Show(val);

	if (val)
	{
		LoadStrings();
	}
}