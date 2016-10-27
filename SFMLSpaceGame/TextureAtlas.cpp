#include "stdafx.h"
#include <TextureAtlas.h>

TextureAtlas::TextureAtlas()
	: m_texture(nullptr)
{
}

TextureAtlas::TextureAtlas(std::shared_ptr<sf::Texture> tex)
	: m_texture(tex) 
{}

sf::Texture* TextureAtlas::GetTexture()
{
	return m_texture.get();
}

void TextureAtlas::SetTexture(std::shared_ptr<sf::Texture> tex)
{
	m_texture = tex;
}

void TextureAtlas::Add(const std::string& name, const sf::IntRect& rect) 
{
	m_rects.emplace(name, rect);
}

sf::IntRect TextureAtlas::operator[](const std::string& name) 
{
	return m_rects[name];
}

sf::IntRect TextureAtlas::at(const std::string& name)
{
	return m_rects.at(name);
}

std::string TextureAtlas::GetTypeName()
{ 
	static const std::string name = "atlas"; 
	return name; 
}