
#include "Camera.h"

namespace Blackguard
{
namespace BurglaryState
{
	
void Camera::setSize(const sf::Vector2u& size)
{
	viewport = sf::View(sf::FloatRect(0,0,size.x,size.y));
}

void Camera::setPosition(const sf::Vector2f& pos)
{
	position = sf::Vector2f(floor(pos.x-0.5), floor(pos.y-0.5));
	viewport.setCenter(position);
}

const sf::View& Camera::getViewport() const
{
	return viewport;
}

}
}