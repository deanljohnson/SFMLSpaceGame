#pragma once
#include <UI/RigEditorWidget.h>

class LaserRig;

class LaserRigEditorWidget : public RigEditorWidget
{
private:
	sfg::Window::Ptr m_window;

	sfg::Entry::Ptr m_fireRateEntry;
	sfg::Entry::Ptr m_heatLimitEntry;
	sfg::Entry::Ptr m_cooldownRateEntry;
	sfg::Entry::Ptr m_heatGeneratedEntry;

	sfg::Entry::Ptr m_projSpeedEntry;
	sfg::Entry::Ptr m_projLifetimeEntry;
	sfg::Entry::Ptr m_projDamageEntry;
	sfg::Entry::Ptr m_projSizeXEntry;
	sfg::Entry::Ptr m_projSizeYEntry;

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