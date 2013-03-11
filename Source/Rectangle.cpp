
#include "Rectangle.h"

using namespace Blackguard;

bool Rectangle::intersects(const Rectangle& other) const
{
	return position.x < other.position.x + other.size.y  &&
		   other.position.x < position.x + size.x  &&
		   position.y < other.position.y + other.size.y &&
		   other.position.y < position.y + size.y;
}