/*
 *   Copyright 2013 Manuel Riecke <m.riecke@mrbeast.org>
 *   Copyright 2013 Raffael Zica <sirjson133@gmail.com>
 *
 *   This software is provided 'as-is', without any express or implied
 *   warranty.  In no event will the authors be held liable for any damages
 *   arising from the use of this software.
 *
 *   Permission is granted to anyone to use this software for any purpose,
 *   including commercial applications, and to alter it and redistribute it
 *   freely, subject to the following restrictions:
 *
 *   1. The origin of this software must not be misrepresented; you must not
 *      claim that you wrote the original software. If you use this software
 *      in a product, an acknowledgment in the product documentation would be
 *      appreciated but is not required.
 *   2. Altered source versions must be plainly marked as such, and must not be
 *      misrepresented as being the original software.
 *   3. This notice may not be removed or altered from any source distribution.
 */

#include "Player.h"
#include "BurglaryState.h"
#include "SFML/Graphics.hpp"
#include "../Game.h"
#include "Camera.h"

using namespace Blackguard;
using namespace Blackguard::BurglaryState;
using namespace Blackguard::Utility;

Player::Player() : Entity()
{
	this->graphics.setTexture(Game::instance->assets.textures["Player"]);
	auto size = graphics.getTexture()->getSize();
	this->bounds.offset = sf::Vector2f(size.x/4, size.y*(2/3.f));
	this->bounds.size = sf::Vector2f(size.x/2, size.y/3);
	this->isMoving = false;
	this->isRunning = false;
	this->movingDir = South;
	this->activationArea.size = sf::Vector2f(25, 25);
}

Player::~Player()
{
}

void Player::update(float deltaTime)
{
	float speed = 64.f;
	if(this->isRunning)
		speed = 128.f;

	auto movementVector = DirToVector(movingDir) * speed * deltaTime;
	if(this->isMoving)
	{
		this->move(movementVector);
		((Camera*)this->world->getNamedEntity("camera"))->setPosition(position);
	}
	((Camera*)this->world->getNamedEntity("camera"))->use();
}

void Player::draw(sf::RenderTarget* target) const
{
	target->draw(graphics);
}

void Player::move(const sf::Vector2f& pos)
{
	Entity::move(pos);
	graphics.setPosition(position);
}

void Player::setPosition(const sf::Vector2f& pos)
{
	Entity::setPosition(pos);
	graphics.setPosition(position);
}

bool Player::activate()
{
	BoundingBox activationRectangle = activationArea.translated(DirToVector(movingDir) * 32.f);
	activationRectangle.position = this->position;
	std::vector<Entity*> objects = world->getEntitiesInsideRect(activationRectangle);
	for(Entity* obj : objects)
	{
		if(obj->activate(*this))
		{
			return true;
		}
	}
	return false;
}

void Player::setMoving(bool moving)
{
	this->isMoving = moving;
}

void Player::setMovingDirection(Direction dir)
{
	if(dir != Stop)
		this->movingDir = dir;
}

void Player::setRunning(bool running)
{
	this->isRunning = running;
}