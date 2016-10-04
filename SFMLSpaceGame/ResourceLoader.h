#pragma once
#include <SFML/Graphics/Image.hpp>
#include <memory>
#include <SFML/Graphics/Texture.hpp>
#include <ShipStats.h>
#include "ProjectileStats.h"

#ifndef ResourceID
#define ResourceID int
#endif
#include <SFML/Graphics/Font.hpp>

void UnloadUnusedResources();

std::shared_ptr<sf::Image> LoadImageResource(ResourceID id);
std::shared_ptr<sf::Texture> LoadTextureResource(ResourceID id);
std::shared_ptr<sf::Font> LoadFont(ResourceID id);
std::shared_ptr<ShipStats> LoadShip(ResourceID id);
std::shared_ptr<ProjectileStats> LoadProjectile(ResourceID id);