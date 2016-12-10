#pragma once
#include <UI/RigEditorWidget.h>

class MissileRig;

class MissileRigEditorWidget : public RigEditorWidget
{
private:
	sfg::Window::Ptr m_window;

	sfg::Entry::Ptr m_fireRateEntry;
	sfg::Entry::Ptr m_thrustEntry;
	sfg::Entry::Ptr m_damageEntry;

	sfg::Button::Ptr m_selectImageButton;
	sfg::Image::Ptr m_missileImageWidget;
	sf::Image m_missileImage;
	std::string m_imageLoc;

	std::shared_ptr<MissileRig> m_missileRig;

	void SetupEntryValidationSignals();
	void SetupEntryFocusSignals();
	void SetupButtonSignals();

	void OpenImageSelector();
	void SetImage(const std::string& imageLoc);

	void ClearEditing();

	bool CheckAllEntryValidity();

	void OnEntryFloatTextValidation(sfg::Entry::Ptr entry);
public:
	explicit MissileRigEditorWidget(const std::string& name);

	virtual void Save() override;
	virtual void Delete() override;
	virtual sfg::Widget::Ptr GetWidget() override;
};