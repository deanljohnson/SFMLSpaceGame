#pragma once
#include <map>
#include <SFML\Graphics\Rect.hpp>
#include <SFML\Graphics\Texture.hpp>
#include <cereal\cereal.hpp>
#include <cereal\types\map.hpp>
#include <cereal\types\vector.hpp>

// Maps keys to a frame within a texture
template<typename TKey>
class TextureMap 
{
private:
	// If TKey is size_t, use a std::vector as the 
	// underlying container type else use std::map
	typedef typename std::conditional<std::is_same<TKey, size_t>::value, 
										std::vector<sf::IntRect>,
										std::map<TKey, sf::IntRect>>::type MapContainer;
	MapContainer m_rects;
	std::shared_ptr<sf::Texture> m_texture;
public:
	explicit TextureMap(std::shared_ptr<sf::Texture> tex = nullptr)
		: m_texture(tex)
	{};

	sf::Texture* GetTexture() { return m_texture.get(); }
	void SetTexture(std::shared_ptr<sf::Texture> tex) { m_texture = tex; }

	void Add(const TKey& key, const sf::IntRect& rect);

	sf::IntRect operator[](const TKey& key)
	{
		return m_rects[key];
	}
	sf::IntRect at(const TKey& key)
	{
		return m_rects.at(key);
	}

	template<class Archive>
	void serialize(Archive& archive) 
	{
		archive(cereal::make_nvp("Rects", m_rects));
	}

	static std::string GetTypeName();
};

template<typename TKey>
void TextureMap<TKey>::Add(const TKey& key, const sf::IntRect& rect)
{
	m_rects.emplace(key, rect);
}

// size_t specialization uses a std::vector as the 
// container so we need a different add method
template<>
void TextureMap<size_t>::Add(const size_t& key, const sf::IntRect& rect);

template<typename TKey>
std::string TextureMap<TKey>::GetTypeName()
{
	static const std::string name = "atlasmap";
	return name;
}

template<>
std::string TextureMap<std::string>::GetTypeName();
template<>
std::string TextureMap<size_t>::GetTypeName();