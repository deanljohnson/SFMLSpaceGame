#include <UI/UI.h>

UI_ID UI::m_nextID = UI_ID_NULL + 1;
long UI::m_updateCounter = 0;
std::unordered_map<UI_ID, UI::ElementRecord> UI::m_elements{};
std::unordered_map<UI_ID, std::shared_ptr<UILayoutOption>> UI::m_layoutOptions{};
std::vector<UI_ID> UI::m_displayOrder{};
std::stack<UI_ID> UI::m_hierarchyIDs{};
std::vector<UI_ID> UI::m_layoutStack{};

void UI::Init()
{
	m_hierarchyIDs.push(UI_ID_NULL);
}

void UI::Update()
{
	m_updateCounter++;

	// erase old UIElements
	for (auto iter = m_elements.begin(); iter != m_elements.end(); )
	{
		if (m_updateCounter - iter->second.lastUpdate > 1)
			m_elements.erase(iter++);
		else ++iter;
	}

	// erase old UILayoutOption's
	for (auto iter = m_layoutOptions.begin(); iter != m_layoutOptions.end(); )
	{
		if (m_updateCounter - iter->second->lastUpdate > 1)
		{
			m_elements.erase(iter->first);
			++iter;
		}
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
				transform *= sf::Transform(parentRecord.element->getTransform());
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

UI_ID UI::CreateLayoutOption(const UILayoutOption& option)
{
	auto id = m_nextID++;
	m_layoutOptions.emplace(make_pair(id, std::make_shared<UILayoutOption>(option)));
	return id;
}

void UI::PushLayoutOption(UI_ID id)
{
	auto& option = m_layoutOptions.find(id)->second;
	option->lastUpdate = m_updateCounter;
	option->Init();
	m_layoutStack.push_back(id);
}

void UI::PopLayoutOption()
{
	auto& option = m_layoutOptions.find(m_layoutStack.back())->second;
	option->Apply();
	m_layoutStack.pop_back();
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
				newStates.transform *= parentRecord.element->getTransform();
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