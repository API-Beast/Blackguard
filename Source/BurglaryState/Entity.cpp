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

#include "Entity.h"
#include "EntityWorldInterface.h"
#include <cmath>

namespace Blackguard
{
namespace BurglaryState
{
	

BoundingBox BoundingBox::translated(const sf::Vector2f& by) const
{
	BoundingBox retVal;
	retVal.position = this->position + by;
	retVal.size = this->size;
	return std::move(retVal);
}	

bool BoundingBox::intersects(const BoundingBox& other) const
{
	return position.x < other.position.x + other.size.y  &&
	       other.position.x < position.x + size.x  &&
	       position.y < other.position.y + other.size.y &&
	       other.position.y < position.y + size.y;
}

void BoundingBox::updatePosition(const sf::Vector2f& entityPosition)
{
	this->position = entityPosition+offset;
}

void Entity::move(const sf::Vector2f& movement)
{
	auto horizontalMovement = sf::Vector2f(movement.x, 0.f       );
	auto verticalMovement   = sf::Vector2f(0.f       , movement.y);
	if(world->isMovementPossible(bounds, movement))
	{
		position += movement;
		bounds.updatePosition(position);
	}
	else if(world->isMovementPossible(bounds, horizontalMovement) && std::abs(horizontalMovement.x) > 0)
	{
		position += horizontalMovement;
		bounds.updatePosition(position);
	}
	else if(world->isMovementPossible(bounds, verticalMovement) && std::abs(verticalMovement.y) > 0)
	{
		position += verticalMovement;
		bounds.updatePosition(position);
	}
	else
		this->onHitWall();
}

void Entity::setPosition(const sf::Vector2f& pos)
{
	position = pos;
	bounds.updatePosition(position);
}

Entity::Entity()
{
	toBeRemoved = false;
}

void Entity::remove()
{
	toBeRemoved = true;
}

void Entity::setWorldInterface(EntityWorldInterface* interface)
{
	this->world = interface;
}

Entity::~Entity()
{

}

}
}