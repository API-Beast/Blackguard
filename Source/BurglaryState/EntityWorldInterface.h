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

#ifndef ENTITYWORLDINTERFACE_H
#define ENTITYWORLDINTERFACE_H

#include "Entity.h"
#include <vector>
#include "../TileMap.h"

namespace Blackguard
{
	namespace BurglaryState
	{		
		class Player;
		class EntityWorldInterface
		{
		public:
			virtual void addEntity(Entity* toAdd)=0;
			virtual Entity* getNamedEntity(const std::string& name)=0;
			virtual std::vector<Entity*> getEntitiesByType(const std::string& type)=0;
			virtual std::vector<Entity*> getEntitiesInsideRect(const Blackguard::BurglaryState::BoundingBox& area)=0;
			virtual std::vector<Entity*> getEntitiesInsideCircle(const Blackguard::BurglaryState::BoundingCircle& area)=0;
			virtual bool isMovementPossible(const BoundingBox& bounds, const sf::Vector2f& movement) const=0;
			virtual RaycastResult raycast(const sf::Vector2f& start, const sf::Vector2f& end) const=0;
			virtual void blockTileAt(const sf::Vector2f& pos)=0;
			virtual void unblockTileAt(const sf::Vector2f& pos)=0;
			virtual void addGoal()=0;
			virtual void markGoalAsReached()=0;
			virtual void onReachedExit()=0;
			virtual void onPlayerWasCatched()=0;
			virtual std::vector<sf::Vector2f> calculatePath(sf::Vector2f start, sf::Vector2f end, bool justInView=false)=0;
			virtual void createNoise(float radius, sf::Vector2f position)=0;
		};
	}
}

#endif