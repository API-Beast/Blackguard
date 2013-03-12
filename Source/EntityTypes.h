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

#ifndef GRG_ENTITYTYPES_H
#define GRG_ENTITYTYPES_H

#include <memory>

namespace Blackguard
{
	class Entity;
	class Player;

	namespace EntityType
	{
		enum : int
		{
			Generic = 0,
			Player = 1,
			Loot = 2
		};
	}

	typedef std::shared_ptr<Entity> EntityPtr;
	typedef std::shared_ptr<Player> PlayerPtr;
}

#endif //GRG_ENTITYTYPES_H