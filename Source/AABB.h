
#ifndef GRG_AABB_H
#define GRG_AABB_H

#include "SFML/System.hpp"

namespace Blackguard
{
	struct AABB
	{
		inline bool intersects(const AABB& other) const;
		sf::Vector2u size;
		sf::Vector2f position;
	};
}

#endif //GRG_AABB_H