
#ifndef GRG_GUARD_H
#define GRG_GUARD_H

#include "Entity.h"
#include "SFML/Graphics.hpp"
#include "Player.h"

namespace sf
{
	class Texture;
}

namespace Blackguard
{
	struct GameData;
	class TileMap;

	namespace BurglaryState
	{
		class Guard : public Entity
		{
		public:
			Guard();
			~Guard();

			virtual void update(float deltaTime);
			virtual void draw(sf::RenderTarget* target) const;
			virtual void move(sf::Vector2f pos);
			virtual void setPosition(sf::Vector2f pos);
			virtual std::string getType(){ return "Guard"; };
		private:
			void chasePlayer();
		private:
			sf::Sprite graphics;
			BoundingBox detectionArea;
		};
	}
}

#endif // GRG_GUARD_H