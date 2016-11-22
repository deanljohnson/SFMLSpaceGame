#include "stdafx.h"
#include <ResourceLoader.h>
#include <resource.h>
#include <windows.h>
#include <map>
#include <assert.h>
#include <ShipStats.h>
#include <ProjectileStats.h>
#include <MissileStats.h>
#include <Serializer.h>
#include <FileSystem.h>

//wrap in anon. namespace to effectively make these private to this file
namespace
{
	Serializer<> serializer = Serializer<>();

	std::map<std::string, std::shared_ptr<TextureMap<std::string>>> loadedStringTextureMaps;
	std::map<std::string, std::shared_ptr<TextureMap<size_t>>> loadedIntTextureMaps;
	std::map<std::string, std::shared_ptr<sf::Texture>> loadedTextures;

	std::map<std::string, std::shared_ptr<ShipStats>> loadedShips;

	std::map<std::string, std::shared_ptr<AnimationDefinition>> loadedAnimations;
	std::map<std::string, std::shared_ptr<ProjectileStats>> loadedProjectiles;
	std::map<std::string, std::shared_ptr<MissileStats>> loadedMissiles;

	std::map<std::string, std::shared_ptr<LaserRig>> loadedLaserRigs;
	std::map<std::string, std::shared_ptr<MissileRig>> loadedMissileRigs;

	// TODO: investigate removing these entirely
	std::map<ResourceID,  std::shared_ptr<AnimationDefinition>> loadedBuiltInAnimations;
	std::map<ResourceID, std::shared_ptr<sf::Font>> loadedFonts;
	std::map<ResourceID, std::shared_ptr<sf::SoundBuffer>> loadedSounds;
	std::map<ResourceID, std::shared_ptr<sf::Texture>> loadedBuiltInTextures;

	bool IsShipID(ResourceID id) 
	{
		return id > SHIP_ID_START && id < SHIP_ID_END;
	}

	bool IsFontID(ResourceID id)
	{
		return id > FONT_ID_START && id < FONT_ID_END;
	}

	bool IsSoundID(ResourceID id)
	{
		return id > SOUND_ID_START && id < SOUND_ID_END;
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
	UnloadUnusedSharedPtrResources(loadedStringTextureMaps);
	UnloadUnusedSharedPtrResources(loadedIntTextureMaps);
	UnloadUnusedSharedPtrResources(loadedTextures);
	UnloadUnusedSharedPtrResources(loadedBuiltInTextures);
	UnloadUnusedSharedPtrResources(loadedAnimations);
	UnloadUnusedSharedPtrResources(loadedBuiltInAnimations);
	UnloadUnusedSharedPtrResources(loadedFonts);
	UnloadUnusedSharedPtrResources(loadedSounds);
	UnloadUnusedSharedPtrResources(loadedProjectiles);
	UnloadUnusedSharedPtrResources(loadedMissiles);
	UnloadUnusedSharedPtrResources(loadedShips);
	UnloadUnusedSharedPtrResources(loadedLaserRigs);
	UnloadUnusedSharedPtrResources(loadedMissileRigs);
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

template<>
std::shared_ptr<TextureMap<std::string>> LoadTextureMap<std::string>(const std::string& name)
{
	//If the resource is already loaded, return it
	auto it = loadedStringTextureMaps.find(name);
	if (it != loadedStringTextureMaps.end())
	{
		return it->second;
	}

	auto tex = LoadTexture(name);

	TextureMap<std::string>* ta = serializer.Load<TextureMap<std::string>>(name);
	ta->SetTexture(tex);

	auto elem = std::shared_ptr<TextureMap<std::string>>(ta);
	loadedStringTextureMaps.insert(make_pair(name, elem));

	return elem;
}

template<>
std::shared_ptr<TextureMap<size_t>> LoadTextureMap<size_t>(const std::string& name)
{
	//If the resource is already loaded, return it
	auto it = loadedIntTextureMaps.find(name);
	if (it != loadedIntTextureMaps.end())
	{
		return it->second;
	}

	auto tex = LoadTexture(name);

	TextureMap<size_t>* ta = serializer.Load<TextureMap<size_t>>(name);
	ta->SetTexture(tex);

	auto elem = std::shared_ptr<TextureMap<size_t>>(ta);
	loadedIntTextureMaps.insert(make_pair(name, elem));

	return elem;
}

template<>
std::shared_ptr<LaserRig> LoadRig<LaserRig>(const std::string& name)
{
	auto it = loadedLaserRigs.find(name);
	if (it != loadedLaserRigs.end())
	{
		return it->second;
	}

	LaserRig* laser = serializer.Load<LaserRig>(name);

	auto elem = std::shared_ptr<LaserRig>(laser);
	loadedLaserRigs.insert(make_pair(name, elem));

	return elem;
}

template<>
std::shared_ptr<MissileRig> LoadRig<MissileRig>(const std::string& name)
{
	auto it = loadedMissileRigs.find(name);
	if (it != loadedMissileRigs.end())
	{
		return it->second;
	}

	MissileRig* missileRig = serializer.Load<MissileRig>(name);

	auto elem = std::shared_ptr<MissileRig>(missileRig);
	loadedMissileRigs.insert(make_pair(name, elem));

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
	else if (id == "exhaust-one")
	{
		animPtr = std::make_shared<AnimationDefinition>(LoadTexture(id), sf::Vector2f(46, 14), 1.f);
	}
	else if (id == "asteroid-one")
	{
		animPtr = std::make_shared<AnimationDefinition>(LoadTexture(id), sf::Vector2f(159, 159), 2.f, 16);
	}
	else
	{
		printf("The given name '%s' does not correspond to a recognized animation\n", id.c_str());
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

std::shared_ptr<ShipStats> LoadShip(const std::string& name)
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

std::shared_ptr<ProjectileStats> LoadProjectile(const std::string& id)
{
	auto it = loadedProjectiles.find(id);
	if (it != loadedProjectiles.end())
	{
		return it->second;
	}

	auto elem = std::make_shared<ProjectileStats>(*serializer.Load<ProjectileStats>(id));
	loadedProjectiles.insert(make_pair(id, elem));
	return elem;
}

std::shared_ptr<MissileStats> LoadMissile(const std::string& id)
{
	auto it = loadedMissiles.find(id);
	if (it != loadedMissiles.end())
	{
		return it->second;
	}

	auto elem = std::make_shared<MissileStats>(*serializer.Load<MissileStats>(id));
	loadedMissiles.insert(make_pair(id, elem));
	return elem;
}