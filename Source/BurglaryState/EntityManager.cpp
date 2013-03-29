/*
 *   Copyright 2013 Manuel Riecke <m.riecke@mrbeast.org>
 *   Copyright 2013 Raffael Zica <sirjson133@gmail.com>
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

#include "../Utility/StringUtilities.h"

#include "EntityManager.h"
#include "Entity.h"

#include <algorithm>
#include <set>
#include <iostream>

using namespace Blackguard;
using namespace Blackguard::Utility;
using namespace Blackguard::BurglaryState;

EntityManager::EntityManager()
{
	idCounter = 0;
}

void EntityManager::addNamed(std::string id, Entity* entity)
{
	namedObjects[id] = entity;
	this->add(entity);
}

void EntityManager::add(Entity* ptr)
{
	objects.push_back(ptr);
	objectsByType[ptr->getType()].push_back(ptr);
}

void EntityManager::cleanup()
{
	std::set<Entity*> toBeDeleted;		
	// Remove from normal list
	for(auto it = objects.begin(); it != objects.end();)
	{
		auto value = *it;
		if(value->canBeRemoved())
		{
			toBeDeleted.insert(value);
			it = objects.erase(it);
		}
		else
			++it;
	}
	
	// Remove from mapped list
	for(auto& pair : objectsByType)
	{
		auto& list = pair.second;
		for(auto it = list.begin(); it != list.end();)
		{
			auto value = *it;
			if(value->canBeRemoved())
			{
				toBeDeleted.insert(value);
				it = list.erase(it);
			}
			else
				++it;
		}
	}
	for(Entity* entity : toBeDeleted)
	{
		//std::cout << "Removed: " << entity << std::endl;
		delete entity;
	}
}

void EntityManager::update(float deltaTime)
{
	for(auto obj : objects)
	{
		obj->update(deltaTime);
	}
	this->cleanup();
}

void EntityManager::draw(sf::RenderTarget* target)
{
	for(auto obj : objects)
	{
		obj->draw(target);
	}
}

void EntityManager::drawLight(sf::RenderTarget* target, sf::RenderStates states)
{
	for(auto obj : objects)
	{
		obj->drawLight(target, states);
	}
}

std::vector< Entity* > EntityManager::getInRect(const BoundingBox& area)
{
	std::vector<Entity*> retValue;
	for(auto obj : objects)
		if(area.intersects(obj->getBounds()))
			retValue.push_back(obj);
	return std::move(retValue);
}

std::vector<Entity*> EntityManager::getInCircle(const BoundingCircle& area)
{
	std::vector<Entity*> retValue;
	for(auto obj : objects)
		if(area.contains(obj->getPosition()))
			retValue.push_back(obj);
	return std::move(retValue);
}

std::vector< Entity* > EntityManager::getByType(const std::string& type)
{
	return objectsByType[type];
}

Entity* EntityManager::getNamed(const std::string& name)
{
	return namedObjects[name];
}