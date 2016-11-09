#include "stdafx.h"
#include <TextureMap.h>

TextureMap::TextureMap()
	: m_texture(nullptr)
{
}

TextureMap::TextureMap(std::shared_ptr<sf::Texture> tex)
	: m_texture(tex) 
{}

sf::Texture* TextureMap::GetTexture()
{
	return m_texture.get();
}

void TextureMap::SetTexture(std::shared_ptr<sf::Texture> tex)
{
	m_texture = tex;
}

void TextureMap::Add(const std::string& name, const sf::IntRect& rect) 
{
	m_rects.emplace(name, rect);
}

sf::IntRect TextureMap::operator[](const std::string& name) 
{
	return m_rects[name];
}

sf::IntRect TextureMap::at(const std::string& name)
{
	return m_rects.at(name);
}

std::string TextureMap::GetTypeName()
{ 
	static const std::string name = "atlasmap"; 
	return name; 
}