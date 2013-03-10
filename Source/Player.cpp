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
#include "SFML/Graphics.hpp"
#include "GameData.h"
#include "Game.h"

using namespace Blackguard;

Player::Player(sf::Texture& texture) : gameData(Game::instance->data)
{
	graphics = sf::Sprite(texture);
	speedModifier = 1;
}

Player::~Player()
{
}

void Player::update(float deltaTime)
{
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
		speedModifier = 3;
	else
		speedModifier = 1;

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		this->move(sf::Vector2f(0,-1 * speedModifier));
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		this->move(sf::Vector2f(0,1 * speedModifier));
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		this->move(sf::Vector2f(-1 * speedModifier,0));
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		this->move(sf::Vector2f(1 * speedModifier,0));

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
		// Here will be activation stuff when a collision system kinda works
	}
}

void Player::draw(sf::RenderTarget* target)
{
	target->draw(graphics);
}

bool Player::processEvent(sf::Event& evt)
{
	return false;
}

void Player::move(sf::Vector2f pos)
{
	Entity::move(pos);
	graphics.setPosition(position);
}

void Player::setPosition(sf::Vector2f pos)
{
	Entity::setPosition(pos);
	graphics.setPosition(position);
}

void Player::addEXP(int value)
{
	gameData.Player.experience += value;
}

void Player::addGold(int value)
{
	gameData.Player.gold += value;
}

int Player::getEXP() const
{
	return gameData.Player.experience;
}

int Player::getGold() const
{
	return gameData.Player.gold;
}

int Player::getLevel() const
{
	// TODO: Better Level formula. Replace placeholder.
	return gameData.Player.experience / 100;
}