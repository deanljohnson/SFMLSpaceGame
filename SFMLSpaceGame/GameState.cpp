// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include <GameState.h>

std::vector<std::unique_ptr<sf::Event>> GameState::pendingEvents{};
EventQueue GameState::pendingGameEvents{};
bool GameState::playerInputLocked{ false };

b2World GameState::world{b2Vec2(0,0)};