#include "NoiseSystem.h"
#include <vector>
#include "EntityWorldInterface.h"
#include "Guard.h"
#include "../Game.h"

using namespace Blackguard;
using namespace Blackguard::BurglaryState;

void NoiseSystem::createNoise(float radius, sf::Vector2f position)
{
	if(radius < 100.f)
		return;
	Noise noise;
	noise.position = position;
	noise.radius = radius;
	noise.time = 0.f;
	noiseData.push_back(noise);
	sf::Texture& tex = Game::instance->assets.textures["Noise"];
	graphics.setTexture(tex);
	graphicsSize = tex.getSize().x/2;
}

void NoiseSystem::update(float deltaTime)
{
	for(Noise& noise : noiseData)
	{
		noise.time+=deltaTime;
	}
	auto it=std::remove_if(noiseData.begin(), noiseData.end(), [](const Noise& a){ return a.time>0.5f; });
	noiseData.erase(it, noiseData.end());
}

void NoiseSystem::clear()
{
	noiseData.clear();
}

void NoiseSystem::draw(sf::RenderTarget* target)
{
	for(const Noise& noise : noiseData)
	{
		graphics.setScale(noise.radius/graphicsSize*noise.time, noise.radius/graphicsSize*noise.time);
		sf::Vector2f origin = sf::Vector2f(graphics.getLocalBounds().width/2, graphics.getLocalBounds().height/2);
		graphics.setOrigin(origin);
		graphics.setPosition(noise.position);
		graphics.setColor(sf::Color(255, 255, 255, 255-255*(noise.time*2)));
		target->draw(graphics);
	}
}
