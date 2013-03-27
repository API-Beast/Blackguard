
#include "Camera.h"

namespace Blackguard
{
	namespace BurglaryState
	{
		Camera::Camera(sf::RenderWindow* window)
		{
			this->window = window;
			sf::Vector2u size = window->getSize();
			viewport = new sf::View(sf::FloatRect(0,0,size.x,size.y));
		}

		Camera::~Camera()
		{
			delete viewport;
		}

		void Camera::use()
		{
			window->setView(*viewport);
		}

		void Camera::move(const sf::Vector2f& pos)
		{
			sf::Vector2f offset = sf::Vector2f((int)pos.x, (int)pos.y);
			position += offset;
			viewport->move(offset);
		}

		void Camera::setPosition(const sf::Vector2f& pos)
		{
			position = sf::Vector2f((int)pos.x, (int)pos.y);
			viewport->setCenter(position);
		}
	}
}