#pragma once
#include <memory>

namespace sf
{
	class Texture;
	class Font;
	class SoundBuffer;
}

class Animation;
class AnimationDefinition;
class ShipStats;

template<typename TMapKey>
class TextureMap;

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