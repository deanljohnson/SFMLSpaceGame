#pragma once
#include <memory>
#include <Item.h>
#include <ItemPrice.h>
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

	void SetItem(Item* item);
	Item* GetItem() const;

	void SetSelected(bool val);

	void SetItemPrice(const ItemPrice& price);

	~InventoryItemWidget();
protected:
	InventoryItemWidget(const std::string& atlas, Item* item);

	std::unique_ptr<sfg::RenderQueue> InvalidateImpl() const override;
	sf::Vector2f CalculateRequisition() override;

private:
	Item* m_item;
	bool m_selected;
	ItemPrice m_price;
	bool m_priceSet;

	static int m_widgetCount;
	static std::shared_ptr<TextureAtlas> m_atlas;
	static std::shared_ptr<sfg::PrimitiveTexture> m_guiTexture;
};