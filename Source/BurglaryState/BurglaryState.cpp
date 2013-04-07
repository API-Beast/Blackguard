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
#include <stdlib.h>

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
	guardCatchedPlayer = false;
	
	sf::RenderWindow* window = Game::instance->getWindow();
	
	{
		sf::Vector2u size = window->getSize();
		if(size.x > 1600)
			size = sf::Vector2u(size.x / 2, size.y / 2);
		camera.setSize(size);
		targetLight.create(size.x, size.y);
		targetLight.setSmooth(true);
	}
	
	blackout.setPosition(0, 0);
	blackout.setSize(sf::Vector2f(window->getSize()));

	gameOver = sf::Sound(Game::instance->assets.sounds["game_over"]);
}

BurglaryState::~BurglaryState()
{
	entities.clear();
}

bool BurglaryState::processEvent(sf::Event& event)
{
	if(levelTime < 1.f) return false;
	if(event.type == sf::Event::KeyPressed)
	{
		if(event.key.code == sf::Keyboard::Space || event.key.code == sf::Keyboard::Return || event.key.code == sf::Keyboard::C)
		{
			if(player)
				player->activate();
			return true;
		}
		if(event.key.code == sf::Keyboard::X)
		{
			player->throwStone();
		}
		if(event.key.code >= sf::Keyboard::F1 && event.key.code <= sf::Keyboard::F12)
		{
			int level = event.key.code - sf::Keyboard::F1;
			if(level < levels.size())
			{
				curLevel = level;
				loadLevel(levels[curLevel]);
			}
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
	levelTime += deltaTime;
	if(levelTime < 1.f) return;
	if(reachedEndOfLevel)
	{
		curLevel++;
		if(curLevel < levels.size())
			loadLevel(levels[curLevel]);
		reachedEndOfLevel = false; // Just for clarification, it's set in loadLevel too.
	}
	if(guardCatchedPlayer)
	{
		gameOver.setVolume(50);
		gameOver.play();
		reloadEntities();
		guardCatchedPlayer = false;
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
	noiseSystem.update(deltaTime);
	entities.update(deltaTime);
	
	if(player)
	{
		// getCenter makes the camera shake again, wtf?
		camera.setPosition(player->getCenter());
	}
}

void BurglaryState::draw(sf::RenderTarget* target)
{
	// The map
	target->setView(camera.getViewport());
	
	tileMap.drawBackground(target);
	entities.drawBackground(target);
	entities.draw(target);
	noiseSystem.draw(target);
	tileMap.drawForeground(target);
	
	targetLight.setView(Game::instance->getWindow()->getView());
	targetLight.clear(sf::Color(10, 20, 60));
	entities.drawLight(&targetLight);
	tileMap.drawShadows(&targetLight, sf::BlendAlpha);
	targetLight.display();
	
	sf::Sprite toDraw;
	toDraw.setTexture(targetLight.getTexture());
	toDraw.setPosition(Game::instance->getWindow()->mapPixelToCoords(sf::Vector2i(0, 0)));
	toDraw.setColor(sf::Color::White);
	target->draw(toDraw, sf::RenderStates(sf::BlendMultiply));
	
	targetLight.setView(target->getView());
	targetLight.clear(sf::Color(19, 33, 55));
	entities.drawLight(&targetLight);
	targetLight.display();
	toDraw.setTexture(targetLight.getTexture());
	toDraw.setColor(sf::Color(48, 36, 16));
	target->draw(toDraw, sf::RenderStates(sf::BlendAdd));
	
	entities.drawGUI(target);
	
	// The stuff that is drawn over the map
	target->setView(target->getDefaultView());
	if(levelTime < 1.f)
	{
		blackout.setFillColor(sf::Color(0, 0, 0, 255-255*levelTime));
		target->draw(blackout);
	}
}

RaycastResult BurglaryState::raycast(const sf::Vector2f& start, const sf::Vector2f& end) const
{
	return tileMap.raycast(start, end);
}

void BurglaryState::createNoise(float radius, sf::Vector2f position)
{
	noiseSystem.createNoise(radius, position);
	auto objs = entities.getInCircle(BoundingCircle(position, radius));
	for(auto obj : objs)
	{
		obj->onNoise(position, radius);
	}
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

std::vector< Entity* > BurglaryState::getEntitiesInsideCircle(const BoundingCircle& area)
{
	return entities.getInCircle(area);
}


Entity* BurglaryState::getNamedEntity(const std::string& name)
{
	return entities.getNamed(name);
}

bool BurglaryState::isMovementPossible(const BoundingBox& bounds, const sf::Vector2f& movement) const
{
	bool a=tileMap.isBlocked(bounds.position+movement);
	bool b=tileMap.isBlocked(bounds.position+bounds.size+movement);
	bool c=tileMap.isBlocked(bounds.position+sf::Vector2f(bounds.size.x, 0)+movement);
	bool d=tileMap.isBlocked(bounds.position+sf::Vector2f(0, bounds.size.y)+movement);
	return (a || b || c || d) == false;
}

void BurglaryState::blockTileAt(const sf::Vector2f& pos)
{
	tileMap.markAsBlocked(pos);
}

void BurglaryState::unblockTileAt(const sf::Vector2f& pos)
{
	tileMap.unblock(pos);
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
	reloadEntities();
}

void BurglaryState::onReachedExit()
{
	reachedEndOfLevel = true;
}

void BurglaryState::onPlayerWasCatched()
{
	guardCatchedPlayer = true;
}

void BurglaryState::reloadEntities()
{
	Game::instance->assets.ambient.play();
	Game::instance->assets.ambient.setLoop(true);
	Game::instance->assets.ambient.setVolume(8);
	entities.clear();
	noiseSystem.clear();
	numberOfGoals = 0;
	reachedGoals  = 0;
	
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
	levelTime = 0.f;
}


void BurglaryState::loadLevels(const vector< string >& levels)
{
	curLevel = 0;
	this->levels = levels;
	loadLevel(levels[curLevel]);
}

vector< sf::Vector2f > BurglaryState::calculatePath(sf::Vector2f start, sf::Vector2f end, bool justInView)
{
	if(raycast(start, end).obstructed)
		return pathFinder.calculatePath(start, end, justInView);
	vector<sf::Vector2f> directPath;
	if(justInView)
		directPath.push_back((start+end)*0.5f);
	else
		directPath.push_back(end);
	return directPath;
}

}
}