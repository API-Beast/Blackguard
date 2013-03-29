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

#ifndef GRG_BURGLARYSTATE_H
#define GRG_BURGLARYSTATE_H

#include "../GameState.h"
#include "../TileMap.h"
#include "EntityManager.h"
#include "EntityWorldInterface.h"

namespace Blackguard
{
	namespace BurglaryState
	{
		class BurglaryState : public GameState, public EntityWorldInterface
		{
		public:
			BurglaryState();
			~BurglaryState();
			// GameState
			virtual bool processEvent(sf::Event& event);
			virtual void update(float deltaTime);
			virtual void draw(sf::RenderTarget* target);
			// EntityWorldInterface
			virtual void addEntity(Entity* toAdd);
			virtual Entity* getNamedEntity(const std::string& name);
			virtual std::vector<Entity*> getEntitiesByType(const std::string& type);
			virtual std::vector<Entity*> getEntitiesInsideRect(const Blackguard::BurglaryState::BoundingBox& area);
			virtual bool isMovementPossible(const BoundingBox& bounds, const sf::Vector2f& movement) const;
			virtual RaycastResult raycast(const sf::Vector2f& start, const sf::Vector2f& distance, float precision=1.5f) const;
			virtual void blockTileAt(const sf::Vector2f& pos);
			virtual void unblockTileAt(const sf::Vector2f& pos);
		private:
			EntityManager* entities;
			Player* player;
			TileMap tileMap;
			sf::RenderTexture targetLight;
		};
	}
}

#endif // GRG_BURGLARYSTATE_H
