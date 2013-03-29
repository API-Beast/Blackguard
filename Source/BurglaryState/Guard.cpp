
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

void Guard::updatePosition()
{
	Blackguard::BurglaryState::Entity::updatePosition();
	graphics.setPosition(position);
}
