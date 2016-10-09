#pragma once
#include <SFML/Graphics/Image.hpp>
#include <memory>
#include <SFML/Graphics/Texture.hpp>
#include <ShipStats.h>
#include "ProjectileStats.h"
#include <SFML/Graphics/Font.hpp>

class Animation;

typedef int ResourceID;
constexpr ResourceID NullResource(-1);

void UnloadUnusedResources();

std::shared_ptr<sf::Image> LoadImageResource(ResourceID id);
std::shared_ptr<sf::Texture> LoadTextureResource(ResourceID id);
std::shared_ptr<Animation> LoadAnimationResource(ResourceID id);
std::shared_ptr<sf::Font> LoadFont(ResourceID id);
std::shared_ptr<ShipStats> LoadShip(ResourceID id);
std::shared_ptr<ProjectileStats> LoadProjectile(ResourceID id);