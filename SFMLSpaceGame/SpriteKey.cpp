// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include <SpriteKey.h>

SpriteKey::SpriteKey()
	: type(Type::Invalid),
	index(0),
	texID("")
{
}

SpriteKey::SpriteKey(const std::string& tex)
	: type(Type::Texture),
	index(0),
	texID(tex)
{
}

SpriteKey::SpriteKey(const std::string& tex, unsigned short index)
	: type(Type::TexIndex),
	index(index),
	texID(tex)
{
}