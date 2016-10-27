#include <UI\InventoryItemWidget.h>
#include <ResourceLoader.h>
#include <SFGUI\RenderQueue.hpp>
#include <SFGUI\Renderer.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/Engine.hpp>
#include <SFML/Graphics/Text.hpp>
#include <UI\StringDisplayHelper.h>

int InventoryItemWidget::m_widgetCount = 0;
std::shared_ptr<TextureAtlas> InventoryItemWidget::m_atlas{ nullptr };
std::shared_ptr<sfg::PrimitiveTexture> InventoryItemWidget::m_guiTexture{ nullptr };

InventoryItemWidget::InventoryItemWidget(const std::string& atlas, Item* item)
	: m_item(item), m_selected(false), m_priceSet(false)
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
	static const std::string name = "InventoryItem";
	return name;
}

void InventoryItemWidget::SetItem(Item* item)
{
	m_item = item;
	Invalidate();
}

Item* InventoryItemWidget::GetItem() const
{
	return m_item;
}

void InventoryItemWidget::SetSelected(bool val)
{
	m_selected = val;
	Invalidate();
}

void InventoryItemWidget::SetItemPrice(const ItemPrice& price) 
{
	m_price = price;
	m_priceSet = true;
	Invalidate();
}

std::unique_ptr<sfg::RenderQueue> InventoryItemWidget::InvalidateImpl() const
{
	std::unique_ptr<sfg::RenderQueue> queue(new sfg::RenderQueue());
	auto* engine = &sfg::Context::Get().GetEngine();

	sf::Vector2f size{ GetAllocation().width, GetAllocation().height };

	auto borderWidth = engine->GetProperty<float>("BorderWidth", shared_from_this());

	// Add a border around the whole widget
	queue->Add(sfg::Renderer::Get().CreatePane(
		{ 0, 0 },
		size,
		borderWidth,
		m_selected ? sf::Color(0x65, 0x67, 0x62) : sf::Color(0x55, 0x57, 0x52),
		sf::Color(0xc6, 0xcb, 0xc4)));

	sf::FloatRect spritePosRect{ 0, 0, 50.f, 50.f };

	sf::IntRect texRect{ m_atlas->at(m_item->GetTypeName()) };
	spritePosRect = sf::FloatRect{ 0, 0, static_cast<float>(texRect.width), static_cast<float>(texRect.height) };
	sf::FloatRect texFloatRect{ static_cast<float>(texRect.left),
		static_cast<float>(texRect.top),
		static_cast<float>(texRect.width),
		static_cast<float>(texRect.height) };
	queue->Add(sfg::Renderer::Get().CreateSprite(spritePosRect, m_guiTexture, texFloatRect));

	auto fontColor = engine->GetProperty<sf::Color>("Color", shared_from_this());
	auto fontSize = engine->GetProperty<unsigned int>("FontSize", shared_from_this());
	auto padding = engine->GetProperty<int>("Padding", shared_from_this());
	const auto& fontName = engine->GetProperty<std::string>("FontName", shared_from_this());
	const auto& font = engine->GetResourceManager().GetFont(fontName);

	auto countSize = engine->GetFontLineHeight(*font, static_cast<int>(fontSize / 1.5f));
	auto countString = StringDisplayHelper::GetRenderedString(m_item->GetAmount());

	sf::Text countText(countString, *font, static_cast<unsigned int>(countSize));
	sf::Vector2f countPosition(
		borderWidth + (padding / 2),
		spritePosRect.height - static_cast<float>(countSize) - borderWidth - (padding / 2)
	);
	countText.setPosition(countPosition);
	countText.setFillColor(fontColor);

	queue->Add(sfg::Renderer::Get().CreateText(countText));

	auto nameSize = engine->GetFontLineHeight(*font, fontSize);
	sf::Text nameText(m_item->GetTypeName(), *font, static_cast<unsigned int>(nameSize));
	sf::Vector2f namePosition(
		(padding * 2) + spritePosRect.width,
		borderWidth + padding
	);
	nameText.setPosition(namePosition);
	nameText.setFillColor(fontColor);

	queue->Add(sfg::Renderer::Get().CreateText(nameText));

	if (m_priceSet) 
	{
		sf::Text priceText(m_price.GetPrice(), *font, static_cast<unsigned int>(nameSize));
		sf::Vector2f pricePosition(
			namePosition.x,
			namePosition.y + nameSize + padding
		);
		priceText.setPosition(pricePosition);
		priceText.setColor(fontColor);

		queue->Add(sfg::Renderer::Get().CreateText(priceText));
	}

	return queue;
}

sf::Vector2f InventoryItemWidget::CalculateRequisition()
{
	return{ 250, 50 };
}