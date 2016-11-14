#include "stdafx.h"
#include <TextureMap.h>

template<>
void TextureMap<size_t>::Add(const size_t& key, const sf::IntRect& rect)
{
	m_rects.insert(m_rects.begin() + key, rect);
}

template<>
std::string TextureMap<size_t>::GetTypeName()
{
	static const std::string name = "atlasmap-i";
	return name;
}

template<>
std::string TextureMap<std::string>::GetTypeName()
{
	static const std::string name = "atlasmap-s";
	return name;
}