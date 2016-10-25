#pragma once
#include <memory>
#include <Item.h>
#include <SFGUI\Widget.hpp>

class InventoryItemWidget : public sfg::Widget
{
public:
	typedef std::shared_ptr<InventoryItemWidget> Ptr;
	typedef std::shared_ptr<const InventoryItemWidget> PtrConst;

	static Ptr Create(const std::string& atlas, Item* item);

	const std::string& GetName() const override;
protected:
	InventoryItemWidget(const std::string& atlas, Item* item);

	std::unique_ptr<sfg::RenderQueue> InvalidateImpl() const override;
	sf::Vector2f CalculateRequisition() override;
};