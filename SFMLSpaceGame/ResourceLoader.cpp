#include <ResourceLoader.h>
#include <resource.h>
#include <windows.h>
#include <map>
#include <assert.h>
#include "ProjectileStats.h"
#include "ShipStats.h"
#include <Serializer.h>
#include <FileSystem.h>

//wrap in anon. namespace to effectively make these private to this file
namespace
{
	Serializer serializer = Serializer();

	std::map<std::string, std::shared_ptr<TextureAtlas>> loadedTextureAtlases;
	std::map<std::string, std::shared_ptr<sf::Texture>> loadedTextures;
	std::map<ResourceID,  std::shared_ptr<sf::Texture>> loadedBuiltInTextures;

	std::map<std::string, std::shared_ptr<ShipStats>> loadedShips;

	std::map<ResourceID,  std::shared_ptr<AnimationDefinition>> loadedBuiltInAnimations;
	std::map<std::string, std::shared_ptr<AnimationDefinition>> loadedAnimations;
	std::map<ResourceID,  std::shared_ptr<sf::Font>> loadedFonts;
	std::map<ResourceID,  std::shared_ptr<sf::SoundBuffer>> loadedSounds;
	std::map<ResourceID,  std::shared_ptr<ProjectileStats>> loadedProjectiles;

	bool IsShipID(ResourceID id) 
	{
		return id > SHIP_ID_START && id < SHIP_ID_END;
	}

	bool IsProjectileID(ResourceID id)
	{
		return id > PROJECTILE_ID_START && id < PROJECTILE_ID_END;
	}

	bool IsFontID(ResourceID id)
	{
		return id > FONT_ID_START && id < FONT_ID_END;
	}

	bool IsSoundID(ResourceID id)
	{
		return id > SOUND_ID_START && id < SOUND_ID_END;
	}

	ProjectileStats* LoadLaserOne()
	{
		return serializer.Load<ProjectileStats>("LaserOne");
	}

	sf::Font* LoadFontOne()
	{
		sf::Font* font = new sf::Font();
		font->loadFromFile("Fonts/Musica.ttf");
		return font;
	}
}

//Takes a map with shared_ptr value and removes elements
//where the map's pointer is the only reference
template<typename KeyType, typename PtrType>
void UnloadUnusedSharedPtrResources(std::map<KeyType, std::shared_ptr<PtrType>>& map)
{
	// Remove all resources with a unique shared_ptr
	for (auto iter = map.begin(); iter != map.end();)
	{
		if (iter->second.unique())
		{
			map.erase(iter++);
		}
		else ++iter;
	}
}

void UnloadUnusedResources()
{
	UnloadUnusedSharedPtrResources(loadedTextureAtlases);
	UnloadUnusedSharedPtrResources(loadedTextures);
	UnloadUnusedSharedPtrResources(loadedBuiltInTextures);
	UnloadUnusedSharedPtrResources(loadedAnimations);
	UnloadUnusedSharedPtrResources(loadedBuiltInAnimations);
	UnloadUnusedSharedPtrResources(loadedFonts);
	UnloadUnusedSharedPtrResources(loadedSounds);
	UnloadUnusedSharedPtrResources(loadedProjectiles);
	UnloadUnusedSharedPtrResources(loadedShips);
}

std::pair<LPVOID, DWORD> LoadRCData(ResourceID id)
{
	HRSRC rsrcData = FindResource(nullptr, MAKEINTRESOURCE(id), RT_RCDATA);
	if (!rsrcData)
		throw std::runtime_error("Failed to find resource.");

	DWORD rsrcDataSize = SizeofResource(nullptr, rsrcData);
	if (rsrcDataSize == 0)
		throw std::runtime_error("Size of resource is 0.");

	HGLOBAL grsrcData = LoadResource(nullptr, rsrcData);
	if (!grsrcData)
		throw std::runtime_error("Failed to load resource.");

	LPVOID firstByte = LockResource(grsrcData);
	if (!firstByte)
		throw std::runtime_error("Failed to lock resource.");

	return std::pair<LPVOID, DWORD>(firstByte, rsrcDataSize);
}

std::shared_ptr<TextureAtlas> LoadTextureAtlas(const std::string& name)
{
	//If the resource is already loaded, return it
	auto it = loadedTextureAtlases.find(name);
	if (it != loadedTextureAtlases.end())
	{
		return it->second;
	}

	auto tex = LoadTexture(name);

	TextureAtlas* ta = serializer.Load<TextureAtlas>(name);
	ta->SetTexture(tex);

	auto elem = std::shared_ptr<TextureAtlas>(ta);
	loadedTextureAtlases.insert(make_pair(name, elem));

	return elem;
}

std::shared_ptr<sf::Texture> LoadTexture(const std::string& name)
{
	//If the resource is already loaded, return it
	auto it = loadedTextures.find(name);
	if (it != loadedTextures.end())
	{
		return it->second;
	}

	auto loadPath = IMAGE_PATH + ((name.find('.') < name.size()) ? name : name + ".png");

	sf::Texture image;
	if (!image.loadFromFile(loadPath))
		throw std::runtime_error("Failed to load image from file " + loadPath);

	auto elem = std::make_shared<sf::Texture>(image);
	loadedTextures.insert(make_pair(name, elem));

	return elem;
}

std::shared_ptr<sf::Texture> LoadTexture(ResourceID id)
{
	//If the resource is already loaded, return it
	auto it = loadedBuiltInTextures.find(id);
	if (it != loadedBuiltInTextures.end())
	{
		return it->second;
	}

	auto dataPair = LoadRCData(id);

	sf::Texture tex;
	if (!tex.loadFromMemory(dataPair.first, dataPair.second))
		throw std::runtime_error("Failed to load image from memory.");

	auto elem = std::make_shared<sf::Texture>(tex);
	loadedBuiltInTextures.insert(make_pair(id, elem));

	return elem;
}

std::shared_ptr<AnimationDefinition> LoadAnimationResource(ResourceID id)
{
	//If the resource is already loaded, return it
	auto it = loadedBuiltInAnimations.find(id);
	if (it != loadedBuiltInAnimations.end())
	{
		return it->second;
	}

	std::shared_ptr<AnimationDefinition> animPtr;
	switch (id)
	{
	case ANIMATION_EXHAUST_ONE:
		animPtr = std::make_shared<AnimationDefinition>(LoadTexture(id), sf::Vector2f(46, 14), 1.f);
		break;
	default:
		printf("The given ResourceID %d does not correspond to a recognized animation\n", id);
		return nullptr;
	}

	loadedBuiltInAnimations.insert(make_pair(id, animPtr));

	return animPtr;
}

std::shared_ptr<AnimationDefinition> LoadAnimationResource(const std::string& id)
{
	//If the resource is already loaded, return it
	auto it = loadedAnimations.find(id);
	if (it != loadedAnimations.end())
	{
		return it->second;
	}

	std::shared_ptr<AnimationDefinition> animPtr;
	if (id == "explosion-one")
	{
		animPtr = std::make_shared<AnimationDefinition>(LoadTexture(id), sf::Vector2f(256, 256), 3.f);
	}

	loadedAnimations.insert(make_pair(id, animPtr));

	return animPtr;
}

std::shared_ptr<sf::Font> LoadFont(ResourceID id)
{
	assert(IsFontID(id));

	auto it = loadedFonts.find(id);
	if (it != loadedFonts.end())
	{
		return it->second;
	}

	if (id == FONT_ONE)
	{
		auto elem = std::make_shared<sf::Font>(*LoadFontOne());
		loadedFonts.insert(make_pair(id, elem));
		return elem;
	}

	return nullptr;
}

std::shared_ptr<sf::SoundBuffer> LoadSoundBuffer(ResourceID id)
{
	assert(IsSoundID(id));

	auto it = loadedSounds.find(id);
	if (it != loadedSounds.end())
	{
		return it->second;
	}

	sf::SoundBuffer soundBuffer;
	switch (id)
	{
	case SOUND_LASER_ONE:
		soundBuffer.loadFromFile("Sounds/laser_one.ogg");
		break;
	default:
		printf("The given ResourceID %d does not correspond to a recognized sound\n", id);
		return nullptr;
	}

	auto elem = std::make_shared<sf::SoundBuffer>(soundBuffer);
	loadedSounds.insert(make_pair(id, elem));

	return elem;
}

std::shared_ptr<ShipStats> LoadShip(std::string name)
{
	auto it = loadedShips.find(name);
	if (it != loadedShips.end())
	{
		return it->second;
	}

	auto elem = std::make_shared<ShipStats>(*serializer.Load<ShipStats>(name));

	loadedShips.insert(make_pair(name, elem));

	return elem;
}

std::shared_ptr<ProjectileStats> LoadProjectile(ResourceID id)
{
	assert(IsProjectileID(id));

	auto it = loadedProjectiles.find(id);
	if (it != loadedProjectiles.end())
	{
		return it->second;
	}

	if (id == PROJECTILE_LASER_ONE)
	{
		auto elem = std::make_shared<ProjectileStats>(*LoadLaserOne());
		loadedProjectiles.insert(make_pair(id, elem));
		return elem;
	}

	return nullptr;
}