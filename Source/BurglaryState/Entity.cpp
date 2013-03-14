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

void Entity::move(const sf::Vector2f& pos)
{
	position += pos;
	bounds.position = position;
}

void Entity::setPosition(const sf::Vector2f& pos)
{
	position = pos;
	bounds.position = position;
}

Entity::Entity()
{
	toBeRemoved = false;
}

void Entity::remove()
{
	toBeRemoved = true;
}

Entity::~Entity()
{

}

}
}