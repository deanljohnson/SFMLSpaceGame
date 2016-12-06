// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include <GameTime.h>
#include <GameStateManager.h>
#include <WorldConstants.h>
#include <GameState.h>
#include <Economy.h>
#include "EntityFactory.h"
#include "UI/UI.h"
#include "UI/GameWindow.h"

sf::RenderWindow* GAME_WINDOW;
GameStateManager* GAME_STATE_MANAGER;
size_t DRAW_CALLS = 0;

sf::VideoMode GetVideoMode()
{
	return sf::VideoMode::getFullscreenModes()[0];
}

int main()
{
	srand(time(nullptr));

	const std::string TITLE = "Space Game";
	sf::RenderWindow window(GetVideoMode(), TITLE);
	window.setFramerateLimit(60);
	GAME_WINDOW = &window;

	UI ui;
	ui.SetSize(window.getSize());

	EntityFactory::Init();
	Economy::Init();

	GameStateManager game_manager{};
	game_manager.Init();
	GAME_STATE_MANAGER = &game_manager;

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

			// The window may be closed by event handlers
			// or by clicking tyhe exit button
			if (!window.isOpen())
				return 0;
		}

		ui.Update(GameTime::deltaTime);
		game_manager.Update();
		
#ifdef _DEBUG
		float updateTime = clock.getElapsedTime().asSeconds();
#endif
		DRAW_CALLS = 0;
		window.clear();
		game_manager.Render(window);
		ui.Display(window);
		window.display();
		
#ifdef _DEBUG
		float renderTime = clock.getElapsedTime().asSeconds() - updateTime;
		GAME_WINDOW->setTitle(TITLE
			+ " Update FPS: " + std::to_string(static_cast<int>(1.f / updateTime))
			+ " Render FPS: " + std::to_string(static_cast<int>(1.f / renderTime))
			+ " Draw Calls: " + std::to_string(DRAW_CALLS));
#endif

		GameTime::deltaTime = clock.getElapsedTime().asSeconds();
		if (GameTime::deltaTime > .25f) //limit jump in case of massive lag
			GameTime::deltaTime = .25f;
		GameTime::totalTime += GameTime::deltaTime;
	}

	return 0;
}