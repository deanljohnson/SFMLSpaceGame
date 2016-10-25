#pragma once
#include <memory>
#include <Item.h>
#include <SFGUI\Widget.hpp>
#include <TextureAtlas.h>
#include <SFGUI\PrimitiveTexture.hpp>

class InventoryItemWidget : public sfg::Widget
{
public:
	typedef std::shared_ptr<InventoryItemWidget> Ptr;
	typedef std::shared_ptr<const InventoryItemWidget> PtrConst;

	static Ptr Create(const std::string& atlas, Item* item);

	const std::string& GetName() const override;

	void SetPosition(const sf::Vector2f& p);

	~InventoryItemWidget();
protected:
	InventoryItemWidget(const std::string& atlas, Item* item);

	std::unique_ptr<sfg::RenderQueue> InvalidateImpl() const override;
	sf::Vector2f CalculateRequisition() override;

private:
	sf::Vector2f m_position;
	Item* m_item;

	static int m_widgetCount;
	static std::shared_ptr<TextureAtlas> m_atlas;
	static std::shared_ptr<sfg::PrimitiveTexture> m_guiTexture;
};