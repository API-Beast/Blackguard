/*
 *   Copyright 2013 Manuel Riecke <m.riecke@mrbeast.org>
 *   Copyright 2013 Raffael Zica <sirjson133@gmail.com>
 *
 *   This software is provided 'as-is', without any express or implied
 *   warranty.  In no event will the authors be held liable for any damages
 *   arising from the use of this software.
 *
 *   Permission is granted to anyone to use this software for any purpose,
 *   including commercial applications, and to alter it and redistribute it
 *   freely, subject to the following restrictions:
 *
 *   1. The origin of this software must not be misrepresented; you must not
 *      claim that you wrote the original software. If you use this software
 *      in a product, an acknowledgment in the product documentation would be
 *      appreciated but is not required.
 *   2. Altered source versions must be plainly marked as such, and must not be
 *      misrepresented as being the original software.
 *   3. This notice may not be removed or altered from any source distribution.
 */

#include "Player.h"
#include "BurglaryState.h"
#include "SFML/Graphics.hpp"
#include "../Game.h"
#include "../Utility/GameMath.h"
#include "Camera.h"
#include "Stone.h"
#include "SFML/Audio/SoundSource.hpp"
#include <SFML/Audio/Music.hpp>

using namespace Blackguard;
using namespace Blackguard::BurglaryState;
using namespace Blackguard::Utility;

Player::Player() : Entity()
{
	graphics.setTexture(Game::instance->assets.textures["Player"]);
	auto size = graphics.getTexture()->getSize();
	bounds.offset = sf::Vector2f(size.x*0.25f, size.y*(0.75f));
	bounds.size = sf::Vector2f(size.x*0.5f, size.y*0.25f);
	activationArea.offset = sf::Vector2f(-(size.x*0.25f), 0);
	activationArea.size = sf::Vector2f(32.f, 32.f);
	isMoving = false;
	isRunning = false;
	movingDir = South;
	lastStoneThrown = 0.f;
	walk = sf::Sound(Game::instance->assets.sounds["walk"]);
	exhaustion = 0.f;
}

Player::~Player()
{
}

void Player::update(float deltaTime)
{
	float speed = 90.f;
	if(this->isRunning)
		speed = std::max(180.f - exhaustion*10.f, 90.f);

	auto movementVector = DirToVector(movingDir) * speed * deltaTime;
	if(this->isMoving)
		this->move(movementVector);
	
	if(this->isMoving && this->isRunning)
	{
		exhaustion += deltaTime;
		stepTimer+=deltaTime;
		if(stepTimer >= 0.35f)
		{
			if(walk.getStatus() != sf::SoundSource::Status::Playing)
				walk.play();
			world->createNoise(300, getCenter());
			stepTimer = 0.f;
		}
	}
	else
	{
		stepTimer = 0.f;
		if(exhaustion > 0.f)
			exhaustion -= deltaTime;
	}
	
	lastStoneThrown += deltaTime;
	activationArea.updatePosition(position + DirToVector(movingDir) * 16.f);
}

void Player::draw(sf::RenderTarget* target) const
{
	target->draw(graphics);
}

void Player::move(const sf::Vector2f& pos)
{
	Entity::move(pos);
	graphics.setPosition(position);
}

void Player::setPosition(const sf::Vector2f& pos)
{
	Entity::setPosition(pos);
	graphics.setPosition(position);
}

bool Player::activate()
{
	std::vector<Entity*> objects = world->getEntitiesInsideRect(activationArea);
	for(Entity* obj : objects)
	{
		if(obj->activate(*this))
		{
			return true;
		}
	}
	return false;
}

void Player::setMoving(bool moving)
{
	this->isMoving = moving;
}

void Player::setMovingDirection(Direction dir)
{
	if(dir != Stop)
		this->movingDir = dir;
}

void Player::setRunning(bool running)
{
	this->isRunning = running;
}

void Player::throwStone()
{
	if(lastStoneThrown > 1.f)
	{
		Stone* stone = new Stone();
		stone->setWorldInterface(world);
		world->addEntity(stone);
		stone->setPosition(this->getCenter());
		stone->toss(DirToVector(movingDir) * 512.f);
		lastStoneThrown = 0.f;
	}
}