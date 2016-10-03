#pragma once
#include <SFML/Graphics/Image.hpp>
#include <memory>
#include <SFML/Graphics/Texture.hpp>
#include <ShipStats.h>

#ifndef ResourceID
#define ResourceID int
#endif

void UnloadUnusedResources();
std::shared_ptr<sf::Image> LoadImageResource(ResourceID id);
std::shared_ptr<sf::Texture> LoadTextureResource(ResourceID id);
std::shared_ptr<ShipStats> LoadShip(ResourceID id);