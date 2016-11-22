#pragma once
#include <memory>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <AnimationDefinition.h>
#include <TextureMap.h>

class Animation;
class ShipStats;
class ProjectileStats;
class MissileStats;

typedef int ResourceID;
constexpr ResourceID NullResource(-1);

void UnloadUnusedResources();

template<typename TMapKey>
std::shared_ptr<TextureMap<TMapKey>> LoadTextureMap(const std::string& name);

template<typename TRig>
std::shared_ptr<TRig> LoadRig(const std::string& name);

std::shared_ptr<sf::Texture> LoadTexture(const std::string& name);
std::shared_ptr<sf::Texture> LoadTexture(ResourceID id);

std::shared_ptr<ShipStats> LoadShip(const std::string& name);

std::shared_ptr<AnimationDefinition> LoadAnimationResource(const std::string& id);
std::shared_ptr<sf::Font> LoadFont(ResourceID id);
std::shared_ptr<sf::SoundBuffer> LoadSoundBuffer(ResourceID id);
std::shared_ptr<ProjectileStats> LoadProjectile(const std::string& id);
std::shared_ptr<MissileStats> LoadMissile(const std::string& id);