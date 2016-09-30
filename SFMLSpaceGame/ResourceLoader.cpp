#include <ResourceLoader.h>
#include <windows.h>
#include <map>

//wrap in anon. namespace to effectively make these private to this file
namespace
{
	std::map<ResourceID, std::shared_ptr<sf::Image>> loadedImages;
	std::map<ResourceID, std::shared_ptr<sf::Texture>> loadedTextures;
}

//Takes a map with shared_ptr value and removes elements
//where the map's pointer is the only reference
template<typename KeyType, typename PtrType>
void UnloadUnusedSharedPtrResources(std::map<KeyType, std::shared_ptr<PtrType>>& map)
{
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
