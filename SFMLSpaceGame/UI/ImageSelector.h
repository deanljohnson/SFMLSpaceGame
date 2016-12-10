#pragma once
#include <UI/GameWindow.h>

class SpriteKey;

class ImageSelector : public GameWindow
{
private:
	bool m_checkForAtlas;
	bool m_selectedHasAtlas;
	std::function<void(const SpriteKey&)> m_callback;

	sfg::ComboBox::Ptr m_imageSelector;
	sfg::ComboBox::Ptr m_atlasKeySelector;

	void OnSelectionChange();
	void OnSelectClicked();
	void OnCancel();
	void CallCallback(const SpriteKey& name);
public:
	ImageSelector();

	// Specifies whether or not the ImageSelector should
	// check for an atlas definition and ask the user to 
	// select a specific key in the atlas. Defaults to true.
	void SetCheckForAtlas(bool val);
	void SetCallback(std::function<void(const SpriteKey&)> callback);
};