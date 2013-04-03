
#ifndef BLACKGUARD_GAME_MATH_H
#define BLACKGUARD_GAME_MATH_H

// SFML has the most shittiest Vector2 class in existance.
// No vector specific functions such as getting the length or normalizing it.
// Missing operator overloads such as operator+= or operator%.
// No operator+(Vector2<T>, T), etc.
// Why are we even using this?
// Sincerly, Beast
#include "SFML/System/Vector2.hpp"
#include <cmath>

namespace Blackguard
{
	namespace Utility
	{
		template <typename T> float VectorLength(const sf::Vector2< T >& source);
		template <typename T> sf::Vector2<T> VectorNormalize(const sf::Vector2<T>& source);
		template <typename T> sf::Vector2<T> VectorUnitAxis(const sf::Vector2<T>& source);
		template <typename T> sf::Vector2<T> VectorFloor(const sf::Vector2<T>& source);
		template <typename T> float Angle(const sf::Vector2<T>& A, const sf::Vector2<T>& B);
		template <typename T> float Distance(const sf::Vector2<T>& A, const sf::Vector2<T>& B);
		inline double PI() { return std::atan(1)*4; }
		bool InsideAngle(float angle, float minAngle, float maxAngle);
		float NormalizeAngle(float angle);
		sf::Vector2f AngleToVector(float angle);
	}
}

#include "GameMath.impl.h"

#endif