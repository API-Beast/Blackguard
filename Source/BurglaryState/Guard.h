
#ifndef GRG_GUARD_H
#define GRG_GUARD_H

#include "Entity.h"
#include "SFML/Graphics.hpp"

namespace sf
{
	class Texture;
}

namespace Blackguard
{
	struct GameData;

	namespace BurglaryState
	{
		class Guard : public Entity
		{
		public:
			Guard(sf::Texture& texture);
			~Guard();

			virtual void update(float deltaTime);
			virtual void draw(sf::RenderTarget* target);
			virtual void move(sf::Vector2f pos);
			virtual void setPosition(sf::Vector2f pos);
			virtual void onCollide(EntityPtr other);
			virtual bool isCollideEnabled();
		private:
			sf::Sprite graphics;
		};
	}
}

#endif // GRG_GUARD_H