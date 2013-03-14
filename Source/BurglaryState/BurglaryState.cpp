/*
 *   Copyright 2013 Manuel Riecke <m.riecke@mrbeast.org>
 *   Copyright 2013 Raffael Zica <raffael@trollgames.de>
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

#include "BurglaryState.h"
#include "Player.h"
#include "Loot.h"

#include "../Utility/Direction.h"

#include "../Game.h"

using namespace Blackguard::BurglaryState;
using namespace Blackguard::Utility;

BurglaryState::BurglaryState()
{
	player = std::shared_ptr<Player>(new Player());
	entities = new EntityManager();
	entities->addNamed("playerEnt", player);
	EntityPtr lootEnt = EntityPtr(new Loot());
	lootEnt->setPosition(sf::Vector2f(100,100));
	entities->add(lootEnt);
	tileMap.loadFromFile("test.tmx");
}

BurglaryState::~BurglaryState()
{
	delete entities;
}

bool BurglaryState::processEvent(sf::Event& event)
{
	if(event.type == sf::Event::KeyPressed)
	{
		if(event.key.code == sf::Keyboard::Space || event.key.code == sf::Keyboard::Return || event.key.code == sf::Keyboard::E)
		{
			player->activate();
			return true;
		}
	}
	return false;
}

void BurglaryState::update(float deltaTime)
{
	bool w, s, a, d;
	w = sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
	s = sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down);
	a = sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
	d = sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
	Direction dir=BoolSetToDir(w, s, a, d);
	bool running = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift);
	player->setMoving(w || s || a || d);
	player->setMovingDirection(dir);
	player->setRunning(running);
	entities->update(deltaTime);
}

void BurglaryState::draw(sf::RenderTarget* target)
{
	target->draw(tileMap);
	entities->draw(target);
}

EntityManager* BurglaryState::getEntityManager()
{
	return entities;
}
