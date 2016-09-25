#include <SFML/Graphics.hpp>
#include "Time.h"
#include "GameStateManager.h"

sf::VideoMode GetVideoMode()
{
	return sf::VideoMode::getFullscreenModes()[0];
}

int main()
{
	sf::RenderWindow window(GetVideoMode(), "SFML Works!");
	
	GameStateManager game_manager{};
	game_manager.Init();

	sf::Clock clock{};

	while (window.isOpen())
	{
		clock.restart();

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}

			game_manager.ProcessEvent(event);
		}

		game_manager.Update();

		window.clear();
		game_manager.Render(window);
		window.display();

		Time::deltaTime = clock.getElapsedTime().asSeconds();
		if (Time::deltaTime > .25f)
			Time::deltaTime = .25f;
	}

	return 0;
}