#pragma once
#include <map>
#include <SFML\Graphics\Rect.hpp>
#include <SFML\Graphics\Texture.hpp>
#include <cereal\cereal.hpp>
#include <cereal\types\map.hpp>

// Maps strings to a frame within a texture
class TextureMap 
{
private:
	std::map<std::string, sf::IntRect> m_rects;
	std::shared_ptr<sf::Texture> m_texture;
public:
	TextureMap();
	explicit TextureMap(std::shared_ptr<sf::Texture> tex);

	sf::Texture* GetTexture();
	void SetTexture(std::shared_ptr<sf::Texture> tex);

	void Add(const std::string& name, const sf::IntRect& rect);

	sf::IntRect operator[](const std::string& name);
	sf::IntRect at(const std::string& name);

	template<class Archive>
	void serialize(Archive& archive) 
	{
		archive(cereal::make_nvp("Rects", m_rects));
	}

	static std::string GetTypeName();
};