
#include "Guard.h"

#include "SFML/Graphics.hpp"
#include "../GameData.h"
#include "../Game.h"
#include "Player.h"
#include "../TileMap.h"
#include "../Utility/GameMath.h"

#define GUARD_DETECT_AREA_SIZE 250

using namespace Blackguard;
using namespace Blackguard::BurglaryState;

Guard::Guard(PlayerPtr player, TileMap& map) : tileMap(map)
{
	graphics = sf::Sprite(Game::instance->assets.textures["Guard"]);
	bounds.size = sf::Vector2f(graphics.getTexture()->getSize());
	detectionArea.size = sf::Vector2f(GUARD_DETECT_AREA_SIZE,GUARD_DETECT_AREA_SIZE);
	playerEnt = player;
}

Guard::~Guard()
{
}

void Guard::onCollide(EntityPtr other)
{
}

void Guard::update(float deltaTime)
{
	if(detectionArea.intersects(playerEnt->getBounds())) {
		this->chasePlayer();
	}
}

void Guard::chasePlayer()
{
	sf::Vector2f direction = Utility::VectorNormalize(playerEnt->getPosition() - this->position);
	if(!tileMap.isBlocked(sf::Vector2i(position + direction))) {
		this->move(direction);
	}
}

bool Guard::isCollideEnabled()
{
	return true;
}

void Guard::draw(sf::RenderTarget* target) const
{
	target->draw(graphics);
}

void Guard::move(sf::Vector2f pos)
{
	Entity::move(pos);
	graphics.setPosition(position);
	detectionArea.position = position - sf::Vector2f(GUARD_DETECT_AREA_SIZE / 2,GUARD_DETECT_AREA_SIZE / 2);
}

void Guard::setPosition(sf::Vector2f pos)
{
	Entity::setPosition(pos);
	graphics.setPosition(position);
	detectionArea.position = position - sf::Vector2f(GUARD_DETECT_AREA_SIZE / 2,GUARD_DETECT_AREA_SIZE / 2);
}