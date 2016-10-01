#include <GameTime.h>
#include <SFML/Graphics.hpp>
#include <GameStateManager.h>
#include <WorldConstants.h>

sf::RenderWindow* GAME_WINDOW;

sf::VideoMode GetVideoMode()
{
	return sf::VideoMode::getFullscreenModes()[0];
}

int main()
{
	sf::RenderWindow window(GetVideoMode(), "SFML Works!");
	window.setFramerateLimit(60);
	
	GAME_WINDOW = &window;

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

		GameTime::deltaTime = clock.getElapsedTime().asSeconds();
		if (GameTime::deltaTime > .25f) //limit jump in case of massive lag
			GameTime::deltaTime = .25f;
		GameTime::totalTime += GameTime::deltaTime;
	}

	return 0;
}