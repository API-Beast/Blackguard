#ifndef BLACKGUARD_NOISE_H
#define BLACKGUARD_NOISE_H

#include <stack>
#include "Entity.h"
#include <SFML/Graphics.hpp>

namespace Blackguard
{
	namespace BurglaryState
	{
		struct Noise : public BoundingCircle
		{
		public:
			float animFadeout;
		};

		class NoiseSystem
		{
		public:
			void createNoise(float radius, sf::Vector2f position); 
			void update(EntityWorldInterface* world);
			void draw(sf::RenderTarget* target);
			void clear();
		private:
			std::stack<Noise> noiseData;
			std::vector<sf::CircleShape> indicatorData;
		};
	}
}

#endif