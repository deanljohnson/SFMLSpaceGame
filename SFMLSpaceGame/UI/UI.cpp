#include <UI/UI.h>

UI_ID UI::m_nextID = UI_ID_NULL + 1;
long UI::m_updateCounter = 0;
std::unordered_map<UI_ID, UI::ElementRecord> UI::m_elements{};
std::vector<UI_ID> UI::m_displayOrder{};
std::stack<UI_ID> UI::m_hierarchyIDs{};
std::vector<UI_ID> UI::m_rootIDs{};

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
	std::stack<sf::Transform> transStack;
	std::stack<short> childCountStack;
	sf::Transform trans;

	/*for (auto id : m_displayOrder)
	{
		auto& record = m_elements.find(id)->second;

		if (!childCountStack.empty())
		{
			childCountStack.top()--;
		}

		response = record.element->HandleEvent(event, trans, &record.result);

		// Push a new transform
		if (record.childCount > 0)
		{
			childCountStack.push(record.childCount);
			transStack.push(record.element->getTransform());

			// Extend the current transform stack
			trans.combine(transStack.top());
		}
		// Pop a transform
		else if (!childCountStack.empty()
			&& childCountStack.top() == 0)
		{
			trans.combine(transStack.top().getInverse());
			transStack.pop();
			childCountStack.pop();
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
	}*/

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
	std::stack<sf::Transform> transStack;
	std::stack<short> childCountStack;

	for (auto id : m_rootIDs) 
	{
		auto& record = m_elements.find(id)->second;
		record.element->Render(target, states);
	}

	m_rootIDs.clear();

	for (auto& id : m_elements)
	{
		id.second.element->UpdateResult(&id.second.result);
	}
}