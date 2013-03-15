
#include "Guard.h"

#include "SFML/Graphics.hpp"
#include "../GameData.h"
#include "../Game.h"

using namespace Blackguard::BurglaryState;

Guard::Guard(sf::Texture& texture)
{
	graphics = sf::Sprite(texture);
	bounds.size = sf::Vector2f(graphics.getTexture()->getSize());
}

Guard::~Guard()
{
}

void Guard::onCollide(EntityPtr other)
{
}

void Guard::update(float deltaTime)
{

}

bool Guard::isCollideEnabled()
{
	return true;
}

void Guard::draw(sf::RenderTarget* target)
{
	target->draw(graphics);
}

void Guard::move(sf::Vector2f pos)
{
	Entity::move(pos);
	graphics.setPosition(position);
}

void Guard::setPosition(sf::Vector2f pos)
{
	Entity::setPosition(pos);
	graphics.setPosition(position);
}