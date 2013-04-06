#include "Stone.h"
#include "../Utility/GameMath.h"
#include "EntityWorldInterface.h"
#include "Guard.h"
#include "../Game.h"
#include "../Utility/GameMath.h"

#include <iostream>

using namespace Blackguard;
using namespace Blackguard::BurglaryState;

Stone::Stone()
{
	this->graphics.setTexture(Game::instance->assets.textures["Stone"]);
	this->shadow.setTexture(Game::instance->assets.textures["Stone/Shadow"]);
	auto size = graphics.getTexture()->getSize();
	this->bounds.size = sf::Vector2f(size.x, size.y);
	stoneHit = sf::Sound(Game::instance->assets.sounds["stone_hit"]);
}

void Stone::update(float deltaTime)
{
	if(Utility::VectorLength(speed) > 0.f)
	{
		height -= 1.0f*deltaTime;
		if(height <= 0)
		{
			height = 0;
			speed = sf::Vector2f(0.f, 0.f);
			stoneHit.setVolume(20);
			stoneHit.play();
		}
		this->move(speed*deltaTime);
	}
}

void Stone::toss(sf::Vector2f speed)
{
	height = 1.f;
	this->speed = speed;
}

void Stone::draw(sf::RenderTarget* target) const
{
	target->draw(graphics);
}

void Stone::drawBackground(sf::RenderTarget* target) const
{
	target->draw(shadow);
}

void Stone::makeNoise()
{
	float power=Utility::VectorLength(speed);
	stoneHit.setVolume(power / 4.f);
	stoneHit.play();
	this->world->createNoise(power/2, getCenter());
}

void Stone::updatePosition()
{
	Blackguard::BurglaryState::Entity::updatePosition();
	graphics.setPosition(this->position.x, this->position.y-height*height*15);
	shadow.setPosition(this->position);
}

void Stone::onHitWall()
{
	this->makeNoise();
	speed = speed*(-0.3f);
}

void Stone::onHitWallSideways(bool y)
{
	if(y)
	{
		makeNoise();
		speed.y = speed.y*(-0.3f);
	}
	else
	{
		makeNoise();
		speed.x = speed.x*(-0.3f);
	}
}

