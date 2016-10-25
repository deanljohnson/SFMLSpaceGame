#include <UI\InventoryItemWidget.h>
#include <SFGUI\RenderQueue.hpp>
#include <SFGUI\Renderer.hpp>

InventoryItemWidget::InventoryItemWidget(const std::string& atlas, Item* item)
{

}

InventoryItemWidget::Ptr InventoryItemWidget::Create(const std::string& atlas, Item* item)
{
}

const std::string& InventoryItemWidget::GetName() const
{
	static const std::string name = "InventoryItemWidget";
	return name;
}

std::unique_ptr<sfg::RenderQueue> InventoryItemWidget::InvalidateImpl() const
{
	std::unique_ptr<sfg::RenderQueue> queue(new sfg::RenderQueue());

	//queue->Add(sfg::Renderer::Get().CreateSprite();
}

sf::Vector2f InventoryItemWidget::CalculateRequisition()
{

}