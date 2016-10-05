#include <UI/UI.h>

UI_ID UI::m_nextID = UI_ID_NULL + 1;
long UI::m_updateCounter = 0;
std::unordered_map<UI_ID, UI::ElementRecord> UI::m_elements{};
std::vector<UI_ID> UI::m_displayOrder{};
std::stack<UI_ID> UI::m_hierarchyIDs{};

void UI::Init()
{
	m_hierarchyIDs.push(UI_ID_NULL);
}

void UI::Update()
{
	m_updateCounter++;

	for (auto iter = m_elements.begin(); iter != m_elements.end(); )
	{
		if (m_updateCounter - iter->second.lastUpdate > 1)
			m_elements.erase(iter++);
		else ++iter;
	}

	m_displayOrder.clear();
}

UI_Result& UI::GetResult(UI_ID id)
{
	assert(m_elements.find(id) != m_elements.end());
	auto& record = m_elements.find(id)->second;
	return record.result;
}

bool UI::HandleEvent(const sf::Event& event)
{
	bool handled = false;
	UIEventResponse response = None;

	for (auto id : m_displayOrder)
	{
		auto& record = m_elements.find(id)->second;

		if (record.parent == UI_ID_NULL) response = record.element->HandleEvent(event, sf::Transform(), &record.result);
		else
		{
			sf::Transform transform;

			UI_ID parentID = record.parent;
			while (parentID != UI_ID_NULL)
			{
				auto& parentRecord = m_elements.find(parentID)->second;
				transform *= parentRecord.element->transform;
				parentID = parentRecord.parent;
			}

			response = record.element->HandleEvent(event, transform, &record.result);
		}

		if (response == Consume)
		{
			handled = true;
			break;
		} 
		else if (response == PassOn)
		{
			handled = true;
		}
	}

	return handled;
}

void UI::PushHierarchy(UI_ID parentID)
{
	m_hierarchyIDs.push(parentID);
}

void UI::PopHierarchy()
{
	m_hierarchyIDs.pop();
}

void UI::Render(sf::RenderTarget& target, sf::RenderStates& states)
{
	for (auto id : m_displayOrder)
	{
		auto& record = m_elements.find(id)->second;

		if (record.parent == UI_ID_NULL) record.element->Render(target, states);
		else
		{
			sf::RenderStates newStates{ states };

			UI_ID parentID = record.parent;
			while (parentID != UI_ID_NULL)
			{
				auto& parentRecord = m_elements.find(parentID)->second;
				newStates.transform *= parentRecord.element->transform;
				parentID = parentRecord.parent;
			}

			record.element->Render(target, newStates);
		}
	}

	for (auto& id : m_elements)
	{
		id.second.element->UpdateResult(&id.second.result);
	}
}