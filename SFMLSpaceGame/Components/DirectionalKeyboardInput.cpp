// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include <Components/DirectionalKeyboardInput.h>
#include <VectorMath.h>

using Key = sf::Keyboard;

DirectionalKeyboardInput::DirectionalKeyboardInput(EntityID ent)
	: Component(ent)
{}

void DirectionalKeyboardInput::Update()
{
	sf::Vector2f dir;

	if (Key::isKeyPressed(Key::W) || Key::isKeyPressed(Key::Up))
		dir.y += -1.f;
	if (Key::isKeyPressed(Key::A) || Key::isKeyPressed(Key::Left))
		dir.x += -1.f;
	if (Key::isKeyPressed(Key::S) || Key::isKeyPressed(Key::Down))
		dir.y += 1.f;
	if (Key::isKeyPressed(Key::D) || Key::isKeyPressed(Key::Right))
		dir.x += 1.f;


	input = Normalize(dir);
}
