#include <UI/UI.h>
#include <assert.h>
#include <SFML/Graphics/RenderStates.hpp>

UI_ID UI::m_nextID = UI_ID_NULL + 1;
long UI::m_updateCounter = 0;
std::unordered_map<UI_ID, UI::ElementRecord> UI::m_elements{};
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
		iter->second.element->children.clear();

		if (m_updateCounter - iter->second.lastUpdate > 1)
			m_elements.erase(iter++);
		else ++iter;
	}

	m_rootIDs.clear();
}

UI_Result* UI::GetResult(UI_ID id)
{
	assert(m_elements.find(id) != m_elements.end());
	auto& record = m_elements.find(id)->second;
	return &record.result;
}

bool UI::HandleEvent(const sf::Event& event)
{
	bool handled = false;
	UIEventResponse response = None;
	sf::Transform trans;

	for (auto id : m_rootIDs) 
	{
		auto& record = m_elements.find(id)->second;
		response = record.element->HandleEvent(event, trans);

		handled |= response != None;
		if (response == Consume) break;
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
	for (auto id : m_rootIDs) 
	{
		auto& record = m_elements.find(id)->second;
		record.element->Render(target, states);
	}

	for (auto& id : m_elements)
	{
		id.second.element->UpdateResult(&id.second.result);
	}
}

void UI::AddElement(UI_ID id, UIElement* elem)
{
	elem->ID = id;
	std::unique_ptr<UIElement> uPtr{ elem };
	m_elements.emplace(std::make_pair(id, ElementRecord(move(uPtr), m_updateCounter)));
}

void UI::InsertIntoHierarchy(UI_ID id, UIElement* elem)
{
	if (m_hierarchyIDs.size() > 1)
	{
		m_elements.find(UI_ID(m_hierarchyIDs.top()))->second.element->children.push_back(elem);
	}
	else {
		m_rootIDs.push_back(id);
	}
}

