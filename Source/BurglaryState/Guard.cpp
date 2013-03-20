
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
}

void Guard::chasePlayer()
{
}

void Guard::draw(sf::RenderTarget* target) const
{
	target->draw(graphics);
}

void Guard::move(const sf::Vector2f& pos)
{
	Entity::move(pos);
	graphics.setPosition(position);
	detectionArea.position = position - sf::Vector2f(GUARD_DETECT_AREA_SIZE / 2,GUARD_DETECT_AREA_SIZE / 2);
}

void Guard::setPosition(const sf::Vector2f& pos)
{
	Entity::setPosition(pos);
	graphics.setPosition(position);
	detectionArea.position = position - sf::Vector2f(GUARD_DETECT_AREA_SIZE / 2,GUARD_DETECT_AREA_SIZE / 2);
}