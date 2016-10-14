#include <UI/ShipEditorWindow.h>
#include "UI.h"

ShipEditorWindow::ShipEditorWindow()
	: GameWindow("ship_editor")
{
	m_window = sfg::Window::Create(sfg::Window::TOPLEVEL | sfg::Window::CLOSE);
	m_window->SetTitle("Ship Editor");
	m_window->SetId("ship_editor");

	UI::Singleton->Add(m_window);

	m_window->GetSignal(sfg::Window::OnMouseEnter).Connect( [this]{ OnMouseEnter(); });
	m_window->GetSignal(sfg::Window::OnMouseLeave).Connect( [this]{ OnMouseLeave(); });
	m_window->GetSignal(sfg::Window::OnCloseButton).Connect( [this]{ Show(false); });
}

void ShipEditorWindow::Show(bool val)
{
	if (m_window->IsLocallyVisible() == val)
		return;

	m_window->Show(val);
}