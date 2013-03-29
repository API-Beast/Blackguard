
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
			virtual bool activate(Player& activator);
			virtual std::string getType(){ return "Loot"; };
		protected:
			virtual void updatePosition();
		private:
			sf::Sprite graphics;
			sf::Vector2f lastPosition;
		};
	}
}

#endif