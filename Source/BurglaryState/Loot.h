
#ifndef GRG_LOOT_H
#define GRG_LOOT_H

#include "Entity.h"
#include <SFML/Graphics.hpp>

namespace Blackguard
{
	namespace BurglaryState
	{
		class EntityManager;

		class Loot : public Entity
		{
		public:
			Loot();
			virtual ~Loot();
			virtual void draw(sf::RenderTarget* target) const;
			virtual void move(const sf::Vector2f& pos);
			virtual void setPosition(const sf::Vector2f& pos);
			virtual bool activate(Player& activator);
		private:
			sf::Sprite graphics;
		};
	}
}

#endif