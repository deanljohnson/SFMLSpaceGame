#pragma once
#include <memory>
#include <SFGUI\Widget.hpp>
#include <SFGUI\PrimitiveTexture.hpp>

template<typename TMapKey>
class TextureMap;
class Item;

class InventoryItemWidget : public sfg::Widget
{
public:
	typedef std::shared_ptr<InventoryItemWidget> Ptr;
	typedef std::shared_ptr<const InventoryItemWidget> PtrConst;

	static Ptr Create(const std::string& atlas, std::shared_ptr<Item> item);

	const std::string& GetName() const override;

	void SetItem(std::shared_ptr<Item> item);
	std::shared_ptr<Item> GetItem() const;

	void SetSelected(bool val);

	void SetItemPrice(unsigned price);

	~InventoryItemWidget();
protected:
	InventoryItemWidget(const std::string& atlas, std::shared_ptr<Item> item);

	std::unique_ptr<sfg::RenderQueue> InvalidateImpl() const override;
	sf::Vector2f CalculateRequisition() override;

private:
	std::shared_ptr<Item> m_item;
	bool m_selected;
	unsigned m_price;
	bool m_priceSet;

	static int m_widgetCount;
	static std::shared_ptr<TextureMap<std::string>> m_atlas;
	static std::shared_ptr<sfg::PrimitiveTexture> m_guiTexture;
};