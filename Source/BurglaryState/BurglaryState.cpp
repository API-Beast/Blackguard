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
#include "Guard.h"

#include "../Utility/Direction.h"

#include "../Game.h"

#include <functional>

using namespace Blackguard::BurglaryState;
using namespace Blackguard::Utility;

BurglaryState::BurglaryState()
{
	tileMap.loadFromFile("newTest.tmx");
	entities = new EntityManager();
	
	std::map<std::string, std::function<Entity*()> > factories;
	factories["Player"] = []() -> Entity* { return new Player(); };
	factories["Loot"]   = []() -> Entity* { return new Loot();   };
	factories["Guard"]  = []() -> Entity* { return new Guard();  };
	for(Blackguard::TileObject object : tileMap.getObjects())
	{
		Entity* newEntity;
		if(!object.type.empty())
		{
			newEntity = factories[object.type]();
			newEntity->setWorldInterface(this);
			newEntity->setPosition(sf::Vector2f(object.x, object.y));
			newEntity->initializeProperties(object.properties);
			if(object.name.empty())
				entities->add(newEntity);
			else
				entities->addNamed(object.name, newEntity);
		}
	}
	player = dynamic_cast<Player*>(entities->getNamed("player").get());
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

void BurglaryState::addEntity(Entity* toAdd)
{
	entities->add(toAdd);
}

std::vector< EntityPtr > BurglaryState::getEntitiesByType(const std::string& type)
{
	return entities->getByType(type);
}

std::vector< EntityPtr > BurglaryState::getEntitiesInsideRect(const BoundingBox& area)
{
	return entities->getInRect(area);
}

EntityPtr BurglaryState::getNamedEntity(const std::string& name)
{
	return entities->getNamed(name);
}

bool BurglaryState::isMovementPossible(const BoundingBox& bounds, const sf::Vector2f& movement) const
{
	bool a=tileMap.isBlocked(sf::Vector2i(bounds.position+movement));
	bool b=tileMap.isBlocked(sf::Vector2i(bounds.position+bounds.size+movement));
	bool c=tileMap.isBlocked(sf::Vector2i(bounds.position+sf::Vector2f(bounds.size.x, 0)+movement));
	bool d=tileMap.isBlocked(sf::Vector2i(bounds.position+sf::Vector2f(0, bounds.size.y)+movement));
	return (a || b || c || d) == false;
}

RaycastResult BurglaryState::raycast(const sf::Vector2f& start, const sf::Vector2f& distance, float precision) const
{
	// TODO
}

void BurglaryState::blockTileAt(const sf::Vector2f& pos)
{
	tileMap.markAsBlocked(sf::Vector2i(pos));
}

void BurglaryState::unblockTileAt(const sf::Vector2f& pos)
{
	tileMap.unblock(sf::Vector2i(pos));
}
