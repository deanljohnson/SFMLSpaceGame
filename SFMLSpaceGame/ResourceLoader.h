#pragma once
#include <windows.h>
#include <memory>
#include <string>
#include <SFML/Graphics/Image.hpp>

inline sf::Image LoadImageResource(const std::wstring& name)
{
	HRSRC rsrcData = FindResource(nullptr, name.c_str(), RT_RCDATA);
	if (!rsrcData)
		throw std::runtime_error("Failed to find resource.");

	DWORD rsrcDataSize = SizeofResource(nullptr, rsrcData);
	if (rsrcDataSize <= 0)
		throw std::runtime_error("Size of resource is 0.");

	HGLOBAL grsrcData = LoadResource(nullptr, rsrcData);
	if (!grsrcData)
		throw std::runtime_error("Failed to load resource.");

	LPVOID firstByte = LockResource(grsrcData);
	if (!firstByte)
		throw std::runtime_error("Failed to lock resource.");

	sf::Image image;
	if (!image.loadFromMemory(firstByte, rsrcDataSize))
		throw std::runtime_error("Failed to load image from memory.");

	return image;
}