
#ifndef BLACKGUARD_STONE_H
#define BLACKGUARD_STONE_H

#include "Entity.h"
#include <SFML/Graphics.hpp>

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
			void toss(sf::Vector2f targetPoint);
			virtual void onHitWall();
			virtual void move(const sf::Vector2f& pos);
			virtual void setPosition(const sf::Vector2f& pos);
		private:
			void makeNoise();
		private:
			bool isHidden;
			bool isFlying;
			bool noiseMade;
			sf::Vector2f target;
			sf::Sprite graphics;
			sf::CircleShape indicator;
			float indicatorFadeout;
		};
	}
}

#endif