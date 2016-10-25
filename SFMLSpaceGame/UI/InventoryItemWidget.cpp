#include <UI\InventoryItemWidget.h>
#include <ResourceLoader.h>
#include <SFGUI\RenderQueue.hpp>
#include <SFGUI\Renderer.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/Engine.hpp>

int InventoryItemWidget::m_widgetCount = 0;
std::shared_ptr<TextureAtlas> InventoryItemWidget::m_atlas{ nullptr };
std::shared_ptr<sfg::PrimitiveTexture> InventoryItemWidget::m_guiTexture{ nullptr };

InventoryItemWidget::InventoryItemWidget(const std::string& atlas, Item* item)
	: m_item(item)
{
	m_widgetCount++;
	if (m_widgetCount == 1)
	{
		m_atlas = LoadTextureAtlas(atlas);
		m_guiTexture = sfg::Renderer::Get().LoadTexture(*m_atlas->GetTexture());
	}
}

InventoryItemWidget::~InventoryItemWidget()
{
	m_widgetCount--;
	if (m_widgetCount == 0)
	{
		// Since there are no more active widgets, reset the 
		// static shared_ptr so we do not keep it alive
		// indefinitely
		m_atlas.reset();
		m_guiTexture.reset();
	}
}

InventoryItemWidget::Ptr InventoryItemWidget::Create(const std::string& atlas, Item* item)
{
	return std::shared_ptr<InventoryItemWidget>(new InventoryItemWidget(atlas, item));
}

const std::string& InventoryItemWidget::GetName() const
{
	static const std::string name = "InventoryItemWidget";
	return name;
}

void InventoryItemWidget::SetPosition(const sf::Vector2f& p)
{
	m_position = p;
	Widget::SetPosition(p);
}

std::unique_ptr<sfg::RenderQueue> InventoryItemWidget::InvalidateImpl() const
{
	std::unique_ptr<sfg::RenderQueue> queue(new sfg::RenderQueue());
	
	sf::IntRect texRect{ m_atlas->at(m_item->GetTypeName()) };
	sf::FloatRect posRect{ m_position.x, m_position.y, static_cast<float>(texRect.width), static_cast<float>(texRect.height)};
	sf::FloatRect texFloatRect{ static_cast<float>(texRect.left), 
								static_cast<float>(texRect.top), 
								static_cast<float>(texRect.width),	
								static_cast<float>(texRect.height) };
	queue->Add(sfg::Renderer::Get().CreateSprite(posRect, m_guiTexture, texFloatRect));

	// Add a border around the image
	queue->Add(sfg::Renderer::Get().CreatePane(
		{ posRect.left, posRect.top }, 
		{ posRect.width, posRect.height }, 
		1.f, 
		sf::Color::Transparent, 
		sfg::Context::Get().GetEngine().GetProperty<sf::Color>("BorderColor", shared_from_this())));

	return queue;
}

sf::Vector2f InventoryItemWidget::CalculateRequisition()
{
	sf::IntRect texRect{ m_atlas->at(m_item->GetTypeName()) };
	return{ static_cast<float>(texRect.width), static_cast<float>(texRect.height) };
}