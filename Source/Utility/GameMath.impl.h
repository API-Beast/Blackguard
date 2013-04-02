#include "GameMath.h"
#include <cmath>

namespace Blackguard
{
	namespace Utility
	{
		template <typename T> float VectorLength(const sf::Vector2<T>& source)
		{
			return sqrt((source.x * source.x) + (source.y * source.y));
		}
		
		template <typename T> sf::Vector2<T> VectorNormalize(const sf::Vector2<T>& source)
		{
			float length = VectorLength(source);
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

		template <typename T> sf::Vector2<T> VectorFloor(const sf::Vector2<T>& source)
		{
			return sf::Vector2<T>((int)source.x,(int)source.y);
		}

		#ifdef WIN32
			#pragma warning(pop)
		#endif
		
		template <typename T> float Angle(const sf::Vector2<T>& a, const sf::Vector2<T>& b)
		{
			return atan2(b.y - a.y, b.x - a.x);
		};
		template <typename T> float Distance(const sf::Vector2<T>& a, const sf::Vector2<T>& b)
		{
			return VectorLength(a - b);
		};
	}
}
