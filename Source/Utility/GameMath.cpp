#include "GameMath.h"

constexpr float Blackguard::Utility::NormalizeAngle(float angle)
{
	return std::fmod(PI()*2 + std::fmod(angle, PI()*2), PI()*2);
}

bool Blackguard::Utility::InsideAngle(float angle, float minAngle, float maxAngle)
{
	angle    = NormalizeAngle(angle);
	minAngle = NormalizeAngle(minAngle);
	maxAngle = NormalizeAngle(maxAngle);

	if(minAngle < maxAngle)
		return minAngle <= angle && maxAngle >= angle;
	return minAngle <= angle || maxAngle >= angle;
}

sf::Vector2f Blackguard::Utility::AngleToVector(float angle)
{
	return sf::Vector2f(cos(angle), sin(angle));
}
