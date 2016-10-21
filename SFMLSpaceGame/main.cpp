#include <GameTime.h>
#include <SFML/Graphics.hpp>
#include <GameStateManager.h>
#include <WorldConstants.h>
#include "EntityFactory.h"
#include "UI/UI.h"
#include "UI/GameWindow.h"

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
	
	UI ui;
	ui.SetSize(window.getSize());

	GAME_WINDOW = &window;

	EntityFactory::Init();

	GameStateManager game_manager{};
	game_manager.Init();

	sf::Clock clock{};

	while (window.isOpen())
	{
		clock.restart();

		sf::Event event;
		while (window.pollEvent(event))
		{
			ui.HandleEvent(event);
			
			if (event.type == sf::Event::Closed)
			{
				game_manager.CleanUp();
				window.close();
				return 0;
			}
			else if (event.type == sf::Event::MouseButtonPressed
				|| event.type == sf::Event::MouseButtonReleased
				|| event.type == sf::Event::MouseMoved)
			{
				if (GameWindow::MouseInWindow())
					continue;
			}
			else if (event.type == sf::Event::KeyReleased
				|| event.type == sf::Event::KeyPressed)
			{
				if (GameWindow::WindowHasKeyboardFocus())
					continue;
			}

			game_manager.ProcessEvent(event);
		}

		ui.Update(GameTime::deltaTime);
		game_manager.Update();
		
#ifdef _DEBUG
		float updateTime = clock.getElapsedTime().asSeconds();
		GAME_WINDOW->setTitle(TITLE + " Update FPS: " + std::to_string(static_cast<int>(1.f / updateTime)));
#endif

		window.clear();
		game_manager.Render(window);
		ui.Display(window);
		window.display();

		GameTime::deltaTime = clock.getElapsedTime().asSeconds();
		if (GameTime::deltaTime > .25f) //limit jump in case of massive lag
			GameTime::deltaTime = .25f;
		GameTime::totalTime += GameTime::deltaTime;
	}

	return 0;
}