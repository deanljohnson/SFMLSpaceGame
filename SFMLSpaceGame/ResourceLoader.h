#pragma once
#include <memory>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include "ProjectileStats.h"
#include "AnimationDefinition.h"


class Animation;
class ShipStats;

typedef int ResourceID;
constexpr ResourceID NullResource(-1);

void UnloadUnusedResources();

std::shared_ptr<sf::Texture> LoadTexture(const std::string& name);
std::shared_ptr<sf::Texture> LoadTexture(ResourceID id);

std::shared_ptr<ShipStats> LoadShip(std::string name);

std::shared_ptr<AnimationDefinition> LoadAnimationResource(ResourceID id);
std::shared_ptr<AnimationDefinition> LoadAnimationResource(const std::string& id);
std::shared_ptr<sf::Font> LoadFont(ResourceID id);
std::shared_ptr<sf::SoundBuffer> LoadSoundBuffer(ResourceID id);
std::shared_ptr<ProjectileStats> LoadProjectile(ResourceID id);