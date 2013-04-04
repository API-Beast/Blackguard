#ifndef BLACKGUARD_NOISE_H
#define BLACKGUARD_NOISE_H

#include <stack>
#include "Entity.h"
#include <SFML/Graphics.hpp>

namespace Blackguard
{
	namespace BurglaryState
	{
		struct Noise
		{
		public:
			sf::Vector2f position;
			float radius;
			float time;
		};

		class NoiseSystem
		{
		public:
			void createNoise(float radius, sf::Vector2f position); 
			void update(float deltaTime);
			void draw(sf::RenderTarget* target);
			void clear();
		private:
			std::vector<Noise> noiseData;
			sf::Sprite graphics;
			float graphicsSize;
		};
	}
}

#endif