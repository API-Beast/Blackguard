
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
			Guard(PlayerPtr player, TileMap& map);
			~Guard();

			virtual void update(float deltaTime);
			virtual void draw(sf::RenderTarget* target) const;
			virtual void move(sf::Vector2f pos);
			virtual void setPosition(sf::Vector2f pos);
			virtual void onCollide(EntityPtr other);
			virtual bool isCollideEnabled();
		private:
			void chasePlayer();
		private:
			sf::Sprite graphics;
			BoundingBox detectionArea;
			PlayerPtr playerEnt;
			TileMap& tileMap;
		};

		typedef std::shared_ptr<Guard> GuardPtr;
	}
}

#endif // GRG_GUARD_H