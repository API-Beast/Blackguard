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

#ifndef BLACKGUARD_UTILITY_DIRECTION_H
#define BLACKGUARD_UTILITY_DIRECTION_H
#include <SFML/System/Vector2.hpp>

namespace Blackguard
{
namespace Utility
{

enum Direction
{
  Stop  = 0,
  North = 1,
  South = 2,
  West  = 4,
  East  = 8,
  NorthWest = North | West,
  NorthEast = North | East,
  SouthWest = South | West,
  SouthEast = South | East
};

Direction BoolSetToDir(bool north, bool south, bool west, bool east);
sf::Vector2f DirToVector(Direction dir);
}
}

#endif // BLACKGUARD_UTILITY_DIRECTION_H
