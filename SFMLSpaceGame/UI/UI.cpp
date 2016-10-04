#include <UI/UI.h>

UI_ID UI::m_nextID = 0;
long UI::m_updateCounter = 0;
std::map<UI_ID, UI::ElementRecord> UI::m_elements{};

void UI::Update()
{
	m_updateCounter++;

	for (auto iter = m_elements.begin(); iter != m_elements.end(); )
	{
		if (m_updateCounter - iter->second.lastUpdate > 1)
			m_elements.erase(iter++);
		else ++iter;
	}
}

void UI::Render(sf::RenderTarget& target, sf::RenderStates& states)
{
	for (auto iter = m_elements.begin(); iter != m_elements.end(); ++iter)
	{
		iter->second.element->Render(target, states);
	}
}

UI_Result* UI::GetResult(UI_ID id)
{
	return nullptr;
}