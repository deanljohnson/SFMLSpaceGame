#pragma once
#include <SFML/Graphics/Image.hpp>
#include <memory>
#include <SFML/Graphics/Texture.hpp>

void UnloadUnusedResources();
std::shared_ptr<sf::Image> LoadImageResource(int id);
std::shared_ptr<sf::Texture> LoadTextureResource(int id);