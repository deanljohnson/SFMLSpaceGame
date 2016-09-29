#pragma once
#include <string>
#include <SFML/Graphics/Image.hpp>
#include <memory>

void UnloadUnusedResources();
std::shared_ptr<sf::Image> LoadImageResource(const std::wstring& name);