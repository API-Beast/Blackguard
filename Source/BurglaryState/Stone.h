
#ifndef BLACKGUARD_STONE_H
#define BLACKGUARD_STONE_H

#include "Entity.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio/Sound.hpp>


namespace Blackguard
{
	namespace BurglaryState
	{
		class Stone : public Entity
		{
		public:
			Stone();
			virtual void update(float deltaTime);
			virtual void draw(sf::RenderTarget* target) const;
			virtual void drawBackground(sf::RenderTarget* target) const;
			void toss(sf::Vector2f targetPoint);
			virtual void onHitWall();
			virtual void onHitWallSideways(bool y);
			virtual void updatePosition();
		private:
			void makeNoise();
		private:
			sf::Vector2f speed;
			float height;
			sf::Sprite graphics;
			sf::Sprite shadow;
			sf::Sound stoneHit;
		};
	}
}

#endif