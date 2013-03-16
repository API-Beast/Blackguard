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

#ifndef GRG_ENTITY_H
#define GRG_ENTITY_H

#include <SFML/System/Vector2.hpp>
#include <memory>
#include <string>

namespace sf
{
	class RenderTarget;
};

namespace Blackguard
{
	namespace BurglaryState
	{
		struct BoundingBox;
		class Player;
		class EntityWorldInterface;
		
		struct BoundingBox
		{
			bool intersects(const BoundingBox& other) const;
			BoundingBox translated(const sf::Vector2f& by) const;
			sf::Vector2f size;
			sf::Vector2f position;
		};
		
		class Entity : public std::enable_shared_from_this<Entity>
		{
		public:
			Entity();
			virtual ~Entity();
			virtual void update(float deltaTime) {}
			virtual void draw(sf::RenderTarget* target) const {}
			virtual bool activate(Player& activator) { return false; }
			virtual void remove();
			virtual bool canBeRemoved() { return toBeRemoved; }
			virtual std::string getType(){ return "unknown"; };
			// 
			virtual void onHitWall(){};
			// Setter:
			virtual void move(const sf::Vector2f& pos);
			virtual void setPosition(const sf::Vector2f& pos);
			virtual void setWorldInterface(EntityWorldInterface* interface);
			// Getter:
			virtual sf::Vector2f getPosition() const { return position; }
			virtual BoundingBox& getBounds() { return bounds; }
		protected:
			sf::Vector2f position;
			BoundingBox bounds;
			bool toBeRemoved;
			EntityWorldInterface* world;
		};
		
		typedef std::shared_ptr<Entity> EntityPtr;
	}
}

#endif //GRG_ENTITY_H