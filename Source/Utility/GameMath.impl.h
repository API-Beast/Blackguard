
#ifndef BLACKGUARD_GAME_MATH_IMPL_H
#define BLACKGUARD_GAME_MATH_IMPL_H

namespace Blackguard
{
	namespace Utility
	{
		template <typename T> sf::Vector2<T> VectorNormalize(const sf::Vector2<T>& source)
		{
			float length = sqrt((source.x * source.x) + (source.y * source.y));
			if (length != 0)
				return sf::Vector2<T>(source.x / length, source.y / length);
			else
				return source;
		}
	}
}

#endif