#include <UI/ShipEditorWindow.h>
#include "UI.h"

ShipEditorWindow::ShipEditorWindow()
{
	m_window = sfg::Window::Create();
	m_window->SetTitle("Ship Editor");
	m_window->SetId("ship_editor");

	UI::Singleton->Add(m_window);

	m_window->GetSignal(sfg::Window::OnMouseEnter).Connect( [this]{ OnMouseEnter(); printf("Entered\n"); });
	m_window->GetSignal(sfg::Window::OnMouseLeave).Connect( [this]{ OnMouseLeave(); printf("Exited\n"); });
}