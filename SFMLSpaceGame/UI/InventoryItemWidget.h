#pragma once
#include <memory>
#include <SFGUI\Widget.hpp>
#include <SFGUI\PrimitiveTexture.hpp>
#include <Item.h>

template<typename TMapKey>
class TextureMap;

class InventoryItemWidget : public sfg::Widget
{
public:
	typedef std::shared_ptr<InventoryItemWidget> Ptr;
	typedef std::shared_ptr<const InventoryItemWidget> PtrConst;

	static Ptr Create(const std::string& atlas, const Item& item);

	const std::string& GetName() const override;

	void SetItem(const Item& item);
	const Item& GetItem() const;

	void SetSelected(bool val);

	void SetItemPrice(unsigned price);

	~InventoryItemWidget();
protected:
	InventoryItemWidget(const std::string& atlas, const Item& item);

	std::unique_ptr<sfg::RenderQueue> InvalidateImpl() const override;
	sf::Vector2f CalculateRequisition() override;

private:
	Item m_item;
	bool m_selected;
	unsigned m_price;
	bool m_priceSet;

	static int m_widgetCount;
	static std::shared_ptr<TextureMap<std::string>> m_atlas;
	static std::shared_ptr<sfg::PrimitiveTexture> m_guiTexture;
};