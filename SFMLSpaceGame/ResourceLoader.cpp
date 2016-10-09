#include <ResourceLoader.h>
#include <resource.h>
#include <windows.h>
#include <map>
#include <assert.h>
#include <Components/DirectionalGun.h>
#include "ProjectileStats.h"

//wrap in anon. namespace to effectively make these private to this file
namespace
{
	std::map<ResourceID, std::shared_ptr<sf::Image>> loadedImages;
	std::map<ResourceID, std::shared_ptr<sf::Texture>> loadedTextures;
	std::map<ResourceID, std::shared_ptr<sf::Font>> loadedFonts;
	std::map<ResourceID, std::shared_ptr<ShipStats>> loadedShips;
	std::map<ResourceID, std::shared_ptr<ProjectileStats>> loadedProjectiles;

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
}

// Hard Coded ship/projectile/font loading functions. Eventually will be removed
namespace
{
	ShipStats* LoadHumanFighter()
	{
		return new ShipStats(5.f, 4.f, 4.5f, 3.5f, 10.f,
							ShipThrust(61.f, 50.f, 30.f, 50.f),
							DirectionalGunData(.1f, 10.f, 3.f, 1.f,
							{
								HardPoint(b2Vec2(.5f, -.1f), 0.f),
								HardPoint(b2Vec2(.5f, .1f), 0.f)
							}));
	}

	ProjectileStats* LoadLaserOne()
	{
		return new ProjectileStats(15.f, 5.f, 5.f, b2Vec2(.3f, .03f));
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
	UnloadUnusedSharedPtrResources(loadedImages);
	UnloadUnusedSharedPtrResources(loadedTextures);
	UnloadUnusedSharedPtrResources(loadedFonts);
	UnloadUnusedSharedPtrResources(loadedShips);
	UnloadUnusedSharedPtrResources(loadedProjectiles);
}

std::pair<LPVOID, DWORD> LoadRCData(ResourceID id)
{
	HRSRC rsrcData = FindResource(nullptr, MAKEINTRESOURCE(id), RT_RCDATA);
	if (!rsrcData)
		throw std::runtime_error("Failed to find resource.");

	DWORD rsrcDataSize = SizeofResource(nullptr, rsrcData);
	if (rsrcDataSize <= 0)
		throw std::runtime_error("Size of resource is 0.");

	HGLOBAL grsrcData = LoadResource(nullptr, rsrcData);
	if (!grsrcData)
		throw std::runtime_error("Failed to load resource.");

	LPVOID firstByte = LockResource(grsrcData);
	if (!firstByte)
		throw std::runtime_error("Failed to lock resource.");

	return std::pair<LPVOID, DWORD>(firstByte, rsrcDataSize);
}

std::shared_ptr<sf::Image> LoadImageResource(ResourceID id)
{
	//If the resource is already loaded, return it
	auto it = loadedImages.find(id);
	if (it != loadedImages.end())
	{
		return it->second;
	}

	auto dataPair = LoadRCData(id);

	sf::Image image;
	if (!image.loadFromMemory(dataPair.first, dataPair.second))
		throw std::runtime_error("Failed to load image from memory.");

	auto elem = std::make_shared<sf::Image>(image);
	loadedImages.insert(make_pair(id, elem));

	return elem;
}

std::shared_ptr<sf::Texture> LoadTextureResource(ResourceID id)
{
	//If the resource is already loaded, return it
	auto it = loadedTextures.find(id);
	if (it != loadedTextures.end())
	{
		return it->second;
	}

	auto dataPair = LoadRCData(id);

	sf::Texture tex;
	if (!tex.loadFromMemory(dataPair.first, dataPair.second))
		throw std::runtime_error("Failed to load image from memory.");

	auto elem = std::make_shared<sf::Texture>(tex);
	loadedTextures.insert(make_pair(id, elem));

	return elem;
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

std::shared_ptr<ShipStats> LoadShip(ResourceID id) 
{
	assert(IsShipID(id));

	auto it = loadedShips.find(id);
	if (it != loadedShips.end())
	{
		return it->second;
	}

	if (id == SHIP_HUMAN_FIGHTER) 
	{
		auto elem = std::make_shared<ShipStats>(*LoadHumanFighter());
		loadedShips.insert(make_pair(id, elem));
		return elem;
	}

	return nullptr;
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