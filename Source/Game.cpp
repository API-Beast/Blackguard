/*
 *   Copyright (C) 2013 Manuel Riecke <m.riecke@mrbeast.org>
 *
 *   This software is provided 'as-is', without any express or implied
 *   warranty.  In no event will the authors be held liable for any damages
 *   arising from the use of this software.
 *
 *   Permission is granted to anyone to use this software for any purpose,
 *   including commercial applications, and to alter it and redistribute it
 *   freely, subject to the following restrictions:
 *
 *   1. The origin of this software must not be misrepresented; you must not
 *      claim that you wrote the original software. If you use this software
 *      in a product, an acknowledgment in the product documentation would be
 *      appreciated but is not required.
 *   2. Altered source versions must be plainly marked as such, and must not be
 *      misrepresented as being the original software.
 *   3. This notice may not be removed or altered from any source distribution.
 */

#include "Game.h"
#include "config.h"

#include "Utility/SwitchParser.h"
#include "BurglaryState/BurglaryState.h"

#include <SFML/Graphics.hpp>

using namespace Blackguard;
using namespace Blackguard::Utility;

Game* Game::instance=NULL;

Game::Game(int argc, char** argv)
{
	Game::instance = this;
	
	// Might want to add a few switches later
	//SwitchParser parser;
	//parser.parseArgv(argc, argv);
	
	window = new sf::RenderWindow(sf::VideoMode(800, 600), "Blackguard");
	window->setVerticalSyncEnabled(true);

	assets.load();

	State.burglary = new BurglaryState::BurglaryState();
	
	currentGameState = State.burglary;
}

Game::~Game()
{
	delete State.burglary;
	delete window;
}

float Game::getDeltaTime() const
{
	return deltaTime;
}

int Game::run()
{
	while(true)
	{
		if(!window->isOpen()) return EXIT_SUCCESS;

		this->deltaTime = clock.restart().asSeconds();

		draw();
		processEvents();
		update();
	}
}

void Game::draw()
{
	window->clear();
	currentGameState->draw(window);
	window->display();
}

void Game::processEvents()
{
	sf::Event event;

	while(window->pollEvent(event))
	{
		if(event.type == sf::Event::Closed)
			window->close();
		if(currentGameState->processEvent(event))
			continue;
		if(event.type == sf::Event::KeyPressed)
		{
			if(event.key.code == sf::Keyboard::Escape)
			{
				window->close();
			}
		}
		
	}
}

void Game::update()
{
	currentGameState->update(deltaTime);
}

sf::RenderWindow* Game::getWindow() const
{
	return window;
}
