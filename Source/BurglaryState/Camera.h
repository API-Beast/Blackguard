

#ifndef CAMERA_H
#define CAMERA_H

#include "Entity.h"
#include "SFML/Graphics.hpp"

namespace Blackguard
{
	namespace BurglaryState
	{
		class Camera
		{
		public:
			void setSize(const sf::Vector2u& size);
			void setPosition(const sf::Vector2f& pos);
			const sf::View& getViewport() const;
		private:
			sf::View viewport;
			sf::Vector2f position;
		};
	}
}

#endif // CAMERA_H