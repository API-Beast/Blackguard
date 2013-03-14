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

#include "Direction.h"

namespace Blackguard
{
namespace Utility
{

Direction BoolSetToDir(bool north, bool south, bool west, bool east)
{
	int vertical   = Stop;
	int horizontal = Stop;

	if(north)
		vertical = North;
	else if(south)
		vertical = South;

	if(west)
		horizontal = West;
	else if(east)
		horizontal = East;

	return Direction(vertical | horizontal);
}

sf::Vector2f DirToVector(Direction dir)
{
	switch(dir)
	{
	case Stop:
		return sf::Vector2f(0.0f,  0.0f);

	case North:
		return sf::Vector2f(0.0f, -1.0f);

	case South:
		return sf::Vector2f(0.0f, +1.0f);

	case West:
		return sf::Vector2f(-1.0f,  0.0f);

	case East:
		return sf::Vector2f(+1.0f,  0.0f);

	case NorthWest:
		return sf::Vector2f(-0.7f, -0.7f);

	case NorthEast:
		return sf::Vector2f(+0.7f, -0.7f);

	case SouthWest:
		return sf::Vector2f(-0.7f, +0.7f);

	case SouthEast:
		return sf::Vector2f(+0.7f, +0.7f);
	}

	return sf::Vector2f(0.0f,  0.0f);
}

}
}

sf::Vector2f operator*(const sf::Vector2f& a, const sf::Vector2f& b)
{
	return sf::Vector2f(a.x * b.x, a.y * b.y);
}
