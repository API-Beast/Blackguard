
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
	bounds.size = sf::Vector2f(graphics.getTexture()->getSize());
	detectionArea.size = sf::Vector2f(GUARD_DETECT_AREA_SIZE,GUARD_DETECT_AREA_SIZE);
}

Guard::~Guard()
{
}

void Guard::update(float deltaTime)
{
	this->chasePlayer();
}

void Guard::chasePlayer()
{
	if(waypoints.size() <= 0) {
		Entity* ent = world->getNamedEntity("player");
		if(ent != nullptr)
			waypoints = world->calculatePath(this->position,ent->getPosition());
	}
	else
	{
		if(this->position != waypoints.top()) {
			printf("Waypoint: {%f | %f}\n",waypoints.top().x,waypoints.top().y);
			printf("My Position: {%f | %f}\n",this->position.x,this->position.y);
			this->move(Utility::VectorUnitAxis(waypoints.top() - this->position));
		}
		else
			this->waypoints.pop();
	}
}

void Guard::move(const sf::Vector2f& pos)
{
	Entity::move(pos);
	printf("Guard Move: {%f | %f}\n",pos.x,pos.y);
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
