
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

		#ifdef WIN32
			#pragma warning(push)
			#pragma warning(disable: 4244) // disable possible loss of data warning because it's here useless
		#endif

		template <typename T> sf::Vector2<T> VectorUnitAxis(const sf::Vector2<T>& source)
		{
			return sf::Vector2<T>((source.x == 0) ? 0 : (source.x > 0) ? 1 : -1,(source.y == 0) ? 0 : (source.y > 0) ? 1 : -1);
		}

		#ifdef WIN32
			#pragma warning(pop)
		#endif
	}
}

#endif