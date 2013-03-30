
#ifndef BLACKGUARD_GAME_MATH_H
#define BLACKGUARD_GAME_MATH_H

#include "SFML/System/Vector2.hpp"
#include <cmath>

namespace Blackguard
{
	namespace Utility
	{
		template <typename T> sf::Vector2<T> VectorNormalize(const sf::Vector2<T>& source);
		template <typename T> sf::Vector2<T> VectorUnitAxis(const sf::Vector2<T>& source);
	}
}

#include "GameMath.impl.h"

#endif