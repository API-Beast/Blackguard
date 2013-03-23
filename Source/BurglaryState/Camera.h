

#ifndef CAMERA_H
#define CAMERA_H

#include "Entity.h"
#include "SFML/Graphics.hpp"

namespace Blackguard
{
	namespace BurglaryState
	{
		class Camera : public Entity
		{
		public:
			Camera(sf::RenderWindow* window);
			~Camera();
			void use();
			virtual void move(const sf::Vector2f& pos);
			virtual void setPosition(const sf::Vector2f& pos);
		private:
			sf::View* viewport;
			sf::RenderWindow* window;
		};
	}
}

#endif // CAMERA_H