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

using namespace Blackguard;
using namespace Blackguard::Utility;
using namespace Blackguard::BurglaryState;

EntityManager::EntityManager()
{
	idCounter = 0;
}

void EntityManager::addNamed(std::string id, EntityPtr entity)
{
	namedObjects[id] = entity;
	objects.push_back(entity);
}

void EntityManager::add(EntityPtr ptr)
{
	objects.push_back(ptr);
}

void EntityManager::cleanup()
{
	auto it = std::remove_if(objects.begin(), objects.end(), [](EntityPtr& obj){ return obj->canBeRemoved(); });
	objects.erase(it, objects.end());
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

std::vector< EntityPtr > EntityManager::findEntitiesInside(const BoundingBox& area)
{
	std::vector<EntityPtr> retValue;
	for(auto obj : objects)
		if(area.intersects(obj->getBounds()))
			retValue.push_back(obj);
	return std::move(retValue);
}