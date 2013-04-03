#include "NoiseSystem.h"
#include <vector>
#include "EntityWorldInterface.h"
#include "Guard.h"

using namespace Blackguard::BurglaryState;

void NoiseSystem::createNoise(float radius, sf::Vector2f position)
{
	Noise noise;
	noise.position = position;
	noise.radius = radius;
	noise.animFadeout = 255;
	noiseData.push(noise);
}

void NoiseSystem::update(EntityWorldInterface* world)
{
	for(auto& indicator : indicatorData)
	{
		float fadeOutVal = indicator.getOutlineColor().a;
		indicator.setOutlineColor(sf::Color(255,0,0,fadeOutVal - 5));
	}

	indicatorData.erase(std::remove_if(indicatorData.begin(), indicatorData.end(), [](const sf::CircleShape& indicator){ return indicator.getOutlineColor().a <= 0; }),indicatorData.end());

	if(noiseData.size() > 0)
	{
		while(noiseData.size() > 0)
		{
			Noise noise = noiseData.top();
			std::vector<Entity*> entities = world->getEntitiesInsideCircle(BoundingCircle(noise.position,noise.radius));
			sf::CircleShape indicator = sf::CircleShape(noise.radius,60U);
			indicator.setFillColor(sf::Color(0,0,0,0));
			indicator.setOutlineThickness(2);
			indicator.setOutlineColor(sf::Color(255,0,0,255));
			indicator.setPosition(noise.position - sf::Vector2f(noise.radius,noise.radius));
			for(auto ent : entities)
			{
				if(ent->getType() == "Guard")
				{
					Guard* guard = (Guard*)ent;
					guard->onNoise(noise.position);
				}
			}
			indicatorData.push_back(indicator);
			noiseData.pop();
		}
	}
}

void NoiseSystem::clear()
{
	noiseData = std::stack<Noise>();
	indicatorData.clear();
}

void NoiseSystem::draw(sf::RenderTarget* target)
{
	for(auto indicator : indicatorData)
	{
		target->draw(indicator);
	}
}
