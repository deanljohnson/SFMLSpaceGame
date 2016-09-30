#include <Components/DirectionalKeyboardInput.h>
#include <SFML/Window/Keyboard.hpp>
#include <VectorMath.h>

using Key = sf::Keyboard;

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
