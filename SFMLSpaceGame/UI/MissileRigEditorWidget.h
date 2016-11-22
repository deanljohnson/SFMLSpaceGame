#pragma once
#include <UI/RigEditorWidget.h>

class MissileRigEditorWidget : public RigEditorWidget
{
private:
	sfg::Window::Ptr m_window;

	sfg::Entry::Ptr m_fireRateEntry;

	std::shared_ptr<MissileRig> m_missileRig;

	void SetupEntryValidationSignals();
	void SetupEntryFocusSignals();

	void ClearEditing();

	bool CheckAllEntryValidity();

	void OnEntryFloatTextValidation(sfg::Entry::Ptr entry);
public:
	explicit MissileRigEditorWidget(const std::string& name);

	virtual void Save() override;
	virtual void Delete() override;
	virtual sfg::Widget::Ptr GetWidget() override;
};