#pragma once
#include <UI/RigEditorWidget.h>

class LaserRigEditorWidget : public RigEditorWidget
{
private:
	const sf::Vector2f ENTRY_SIZE = sf::Vector2f(50, 0);

	sfg::Window::Ptr m_window;

	sfg::Entry::Ptr m_fireRateEntry;
	sfg::Entry::Ptr m_heatLimitEntry;
	sfg::Entry::Ptr m_cooldownRateEntry;
	sfg::Entry::Ptr m_heatGeneratedEntry;

	std::shared_ptr<LaserRig> m_laserRig;

	void SetupEntryValidationSignals();
	void SetupEntryFocusSignals();

	void ClearEditing();

	bool CheckAllEntryValidity();

	void OnEntryFloatTextValidation(sfg::Entry::Ptr entry);
public:
	explicit LaserRigEditorWidget(const std::string& name);

	virtual void Save() override;
	virtual void Delete() override;
	virtual sfg::Widget::Ptr GetWidget() override;
};