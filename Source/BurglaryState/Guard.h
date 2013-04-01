
#ifndef GRG_GUARD_H
#define GRG_GUARD_H

#include "Entity.h"
#include "SFML/Graphics.hpp"
#include "Player.h"
#include <stack>

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
			virtual void move(const sf::Vector2f& pos);
			virtual std::string getType(){ return "Guard"; };
			void onNoise(sf::Vector2f source);
		protected:
			virtual void updatePosition();
		private:
			void chasePlayer();
		private:
			sf::Sprite graphics;
			std::stack<sf::Vector2f> waypoints;
		};
	}
}

#endif // GRG_GUARD_H