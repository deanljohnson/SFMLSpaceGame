#include <GameTime.h>
#include <SFML/Graphics.hpp>
#include <GameStateManager.h>
#include <WorldConstants.h>
#include "UI/UI.h"

sf::RenderWindow* GAME_WINDOW;

sf::VideoMode GetVideoMode()
{
	return sf::VideoMode::getFullscreenModes()[0];
}

int main()
{
	const std::string TITLE = "Space Game";
	sf::RenderWindow window(GetVideoMode(), TITLE);
	window.setFramerateLimit(60);
	
	GAME_WINDOW = &window;
	UI::Init();

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
				exit(0);
			}

			game_manager.ProcessEvent(event);
		}

		game_manager.Update();
		
#ifdef _DEBUG
		float updateTime = clock.getElapsedTime().asSeconds();
		GAME_WINDOW->setTitle(TITLE + " Update FPS: " + std::to_string(static_cast<int>(1.f / updateTime)));
#endif

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