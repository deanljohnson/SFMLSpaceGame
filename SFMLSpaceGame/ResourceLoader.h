#pragma once
#include <SFML/Graphics/Image.hpp>
#include <memory>
#include <SFML/Graphics/Texture.hpp>

#define ResourceID int

void UnloadUnusedResources();
std::shared_ptr<sf::Image> LoadImageResource(ResourceID id);
std::shared_ptr<sf::Texture> LoadTextureResource(ResourceID id);