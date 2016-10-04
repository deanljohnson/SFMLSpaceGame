#include <UI/UIManager.h>

void UIManager::Update()
{
	m_updateCounter++;

	for (auto iter = m_elements.begin(); iter != m_elements.end(); )
	{
		if (m_updateCounter - iter->second.lastUpdate > 1)
			m_elements.erase(iter++);
		else ++iter;
	}
}

void UIManager::Render(sf::RenderTarget& target, sf::RenderStates& states)
{
	for (auto iter = m_elements.begin(); iter != m_elements.end(); ++iter)
	{
		iter->second.element->Render(target, states);
	}
}

UI_Result* UIManager::GetResult(UI_ID id)
{
	return nullptr;
}