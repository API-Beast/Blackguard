
#include "Guard.h"

#include "SFML/Graphics.hpp"
#include "../GameData.h"
#include "../Game.h"
#include "Player.h"
#include "../TileMap.h"
#include "../Utility/GameMath.h"
#include "EntityWorldInterface.h"
#include "../PathFinder.h"

#define GUARD_DETECT_AREA_SIZE 250

using namespace Blackguard;
using namespace Blackguard::BurglaryState;

Guard::Guard() : Entity()
{
	graphics = sf::Sprite(Game::instance->assets.textures["Guard"]);
	sf::Vector2u size = graphics.getTexture()->getSize();
	bounds.offset = sf::Vector2f(size.x/4, size.y*(2/3.f));
	bounds.size = sf::Vector2f(size.x/2, size.y/3);
	detectionArea.size = sf::Vector2f(GUARD_DETECT_AREA_SIZE,GUARD_DETECT_AREA_SIZE);
}

Guard::~Guard()
{
}

void Guard::update(float deltaTime)
{
	this->chasePlayer();
}

void Guard::onNoise(sf::Vector2f source)
{
	if(!world->isPathBlocked(this->position,source))
	{
		waypoints = std::stack<sf::Vector2f>();
		waypoints.push(source);
	}
	else
	{
		waypoints = world->calculatePath(this->position,source);
	}
}

void Guard::chasePlayer()
{
	Entity* ent = world->getNamedEntity("player");
	if(!world->isPathBlocked(this->position,ent->getPosition()))
	{
		if(waypoints.size() <= 0) {
			waypoints = std::stack<sf::Vector2f>();
		}
		this->move(Utility::VectorUnitAxis(ent->getPosition() - this->position));
	}
	else
	{
		if(waypoints.size() <= 0) {
			//if(ent != nullptr)
			//	waypoints = world->calculatePath(this->position,ent->getPosition());
		}
		else
		{
			if(this->position != waypoints.top()) {
				this->move(Utility::VectorUnitAxis(waypoints.top() - this->position));
			}
			else
				this->waypoints.pop();
		}
	}

}

void Guard::move(const sf::Vector2f& pos)
{
	Entity::move(pos);
}

void Guard::draw(sf::RenderTarget* target) const
{
	target->draw(graphics);
}

void Guard::updatePosition()
{
	Blackguard::BurglaryState::Entity::updatePosition();
	graphics.setPosition(position);
}
