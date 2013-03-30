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
#include "Camera.h"
#include "Light.h"
#include "Exit.h"

#include "../Utility/Direction.h"

#include "../Game.h"
#include "SFML/System/Vector2.hpp"

#include <functional>
#include <iostream>
using namespace std;

using namespace Blackguard;
using namespace Utility;
namespace Blackguard
{
namespace BurglaryState
{

BurglaryState::BurglaryState() : pathFinder(tileMap)
{
	numberOfGoals = 0;
	reachedGoals = 0;
	levels = std::vector<std::string>();
	curLevel = 0;
	reachedEndOfLevel = false;
}

BurglaryState::~BurglaryState()
{
}

bool BurglaryState::processEvent(sf::Event& event)
{
	if(event.type == sf::Event::KeyPressed)
	{
		if(event.key.code == sf::Keyboard::Space || event.key.code == sf::Keyboard::Return || event.key.code == sf::Keyboard::E)
		{
			if(player)
				player->activate();
			return true;
		}
		/*
		if(event.key.code == sf::Keyboard::K)
		{
			PathFinder finder(&tileMap);
			auto output = finder.calculatePath(sf::Vector2f(192,224), sf::Vector2f(256,416));
			
			while(output.size() > 0)
			{
				printf("{%f | %f}\n",output.top().x,output.top().y);
				output.pop();
			}
			return true;
		}*/
	}
	return false;
}

void BurglaryState::update(float deltaTime)
{
	if(reachedEndOfLevel)
	{
		curLevel++;
		if(curLevel < levels.size())
			loadLevel(levels[curLevel]);
		reachedEndOfLevel = false; // Just for clarification, it's set in loadLevel too.
	}
	if(player)
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
	}
	entities.update(deltaTime);
}

void BurglaryState::draw(sf::RenderTarget* target)
{
	tileMap.drawBackground(target);
	entities.draw(target);
	tileMap.drawForeground(target);
	
	targetLight.setView(Game::instance->getWindow()->getView());
	targetLight.clear(sf::Color(19, 33, 55));
	entities.drawLight(&targetLight);
	tileMap.drawShadows(&targetLight, sf::BlendAlpha);
	targetLight.display();
	
	sf::Sprite toDraw;
	toDraw.setTexture(targetLight.getTexture());
	toDraw.setPosition(Game::instance->getWindow()->mapPixelToCoords(sf::Vector2i(0, 0)));
	toDraw.setColor(sf::Color::White);
	target->draw(toDraw, sf::RenderStates(sf::BlendMultiply));
	
	
	targetLight.setView(Game::instance->getWindow()->getView());
	targetLight.clear(sf::Color(19, 33, 55));
	entities.drawLight(&targetLight);
	targetLight.display();
	toDraw.setTexture(targetLight.getTexture());
	toDraw.setColor(sf::Color(32, 48, 16));
	target->draw(toDraw, sf::RenderStates(sf::BlendAdd));
	
	entities.drawGUI(target);
}

void BurglaryState::addEntity(Entity* toAdd)
{
	entities.add(toAdd);
}

std::vector< Entity* > BurglaryState::getEntitiesByType(const std::string& type)
{
	return entities.getByType(type);
}

std::vector< Entity* > BurglaryState::getEntitiesInsideRect(const BoundingBox& area)
{
	return entities.getInRect(area);
}

Entity* BurglaryState::getNamedEntity(const std::string& name)
{
	return entities.getNamed(name);
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
	return RaycastResult();
}

void BurglaryState::blockTileAt(const sf::Vector2f& pos)
{
	tileMap.markAsBlocked(sf::Vector2i(pos));
}

void BurglaryState::unblockTileAt(const sf::Vector2f& pos)
{
	tileMap.unblock(sf::Vector2i(pos));
}

void BurglaryState::addGoal()
{
	numberOfGoals++;
}

void BurglaryState::markGoalAsReached()
{
	reachedGoals++;
	if(reachedGoals >= numberOfGoals)
	{
		std::cout << "Reached all " << numberOfGoals << " goals." << std::endl;
		for(Entity* exit : entities.getByType("Exit"))
			dynamic_cast<Exit*>(exit)->enable();
	}
}

void BurglaryState::loadLevel(const std::string& level)
{
	tileMap.loadFromFile(level);
	// This doesn't work. WTF???!
	//delete entities;
	//entities = new EntityManager();
	entities.clear();
	
	sf::RenderWindow* window = Game::instance->getWindow();

	entities.addNamed("camera", new Camera(Game::instance->getWindow()));
	targetLight.create(window->getSize().x, window->getSize().y);
	
	std::map<std::string, std::function<Entity*()> > factories;
	factories["Player"] = []() -> Entity* { return new Player(); };
	factories["Loot"]   = []() -> Entity* { return new Loot();   };
	factories["Guard"]  = []() -> Entity* { return new Guard();  };
	factories["Light"]  = []() -> Entity* { return new Light();  };
	factories["Exit"]   = []() -> Entity* { return new Exit();   };
	for(Blackguard::TileObject object : tileMap.getObjects())
	{
		// Entities
		Entity* newEntity;
		if(!object.type.empty())
		{
			newEntity = factories[object.type]();
			newEntity->setWorldInterface(this);
			newEntity->setPosition(sf::Vector2f(object.x, object.y - tileMap.getGridSize().y));
			newEntity->initializeFromTileObject(object);
			cout << "Added " << newEntity << " of type " << object.type << endl;
			if(object.name.empty())
				entities.add(newEntity);
			else
				entities.addNamed(object.name, newEntity);
		}
	}
	player = dynamic_cast<Player*>(entities.getNamed("player"));
	if(player == nullptr) cout << "WARNING: No Player named \"player\"" << endl;
	reachedEndOfLevel = false;
}

void BurglaryState::onReachedExit()
{
	reachedEndOfLevel = true;
}

void BurglaryState::loadLevels(const vector< string >& levels)
{
	curLevel = 0;
	this->levels = levels;
	loadLevel(levels[curLevel]);
}

std::stack<sf::Vector2f> BurglaryState::calculatePath(sf::Vector2f start, sf::Vector2f end)
{
	return pathFinder.calculatePath(start,end);
}

}
}