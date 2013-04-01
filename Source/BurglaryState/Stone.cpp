#include "Stone.h"
#include "../Utility/GameMath.h"
#include "EntityWorldInterface.h"
#include "Guard.h"
#include "../Game.h"

using namespace Blackguard::BurglaryState;

#define STONE_NOISE_RADIUS 100

Stone::Stone()
{
	isHidden = true;
	isFlying = false;
	noiseMade = false;
	this->graphics.setTexture(Game::instance->assets.textures["Stone"]);
	auto size = graphics.getTexture()->getSize();
	this->bounds.offset = sf::Vector2f(size.x - size.x / 2, size.y - size.y / 2);
	this->bounds.size = sf::Vector2f(size.x, size.y);
	indicatorFadeout = 255;
}

void Stone::update(float deltaTime)
{
	if(isFlying) {
		this->move(Utility::VectorUnitAxis(target - this->position));
		if(this->position == target) {
			isFlying = false;
			if(!noiseMade) this->makeNoise();
		}
	}
	if(this->indicatorFadeout > 0) {
		indicator.setOutlineColor(sf::Color(255,0,0,indicatorFadeout));
		this->indicatorFadeout -= 5;
	}
}

void Stone::toss(sf::Vector2f targetPoint)
{
	target = Utility::VectorFloor(targetPoint);
	isHidden = false;
	isFlying = true;
	noiseMade = false;
}

void Stone::draw(sf::RenderTarget* target) const
{
	if(!isHidden)
	{
		target->draw(graphics);
		target->draw(indicator);
	}
}

void Stone::move(const sf::Vector2f& pos)
{
	Entity::move(pos);
	graphics.setPosition(position);
}

void Stone::setPosition(const sf::Vector2f& pos)
{
	Entity::setPosition(pos);
	graphics.setPosition(position);
}

void Stone::makeNoise()
{
	noiseMade = true;
	std::vector<Entity*> entities = this->world->getEntitiesInsideCircle(BoundingCircle(this->position,STONE_NOISE_RADIUS));
	this->indicatorFadeout = 255;
	indicator = sf::CircleShape(STONE_NOISE_RADIUS,60U);
	indicator.setFillColor(sf::Color(0,0,0,0));
	indicator.setOutlineThickness(2);
	indicator.setOutlineColor(sf::Color(255,0,0,indicatorFadeout));
	indicator.setPosition(this->position - sf::Vector2f(STONE_NOISE_RADIUS,STONE_NOISE_RADIUS) + this->bounds.offset);
	for(auto ent : entities)
	{
		if(ent->getType() == "Guard")
		{
			Guard* guard = (Guard*)ent;
			guard->onNoise(this->position);
		}
	}
}

void Stone::onHitWall()
{
	isFlying = false;
	if(!noiseMade) this->makeNoise();
}
