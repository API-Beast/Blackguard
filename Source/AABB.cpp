
#include "AABB.h"

using namespace Blackguard;

inline bool AABB::intersects(const AABB& other) const
{
	return position.x < other.position.x + other.size.y  &&
		   other.position.x < position.x + size.x  &&
		   position.y < other.position.y + other.size.y &&
		   other.position.y < position.y + size.y;
}