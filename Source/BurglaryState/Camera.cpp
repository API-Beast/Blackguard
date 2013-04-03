
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
			sf::Vector2f offset = sf::Vector2f(floor(pos.x-0.5), floor(pos.y-0.5));
			position += offset;
			viewport->move(offset);
		}

		void Camera::setPosition(const sf::Vector2f& pos)
		{
			position = sf::Vector2f(floor(pos.x-0.5), floor(pos.y-0.5));
			viewport->setCenter(position);
		}
	}
}