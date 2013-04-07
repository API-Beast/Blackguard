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

#include "Exit.h"
#include "EntityWorldInterface.h"
#include "Player.h"
#include "../TileMap.h"
#include "../Game.h"


Blackguard::BurglaryState::Exit::Exit()
{
	enabled = false;
	graphics.setTexture(Game::instance->assets.textures["Exit"]);
}

void Blackguard::BurglaryState::Exit::drawGUI(sf::RenderTarget* target) const
{
	if(enabled)
	{
		target->draw(graphics);
	}
}

void Blackguard::BurglaryState::Exit::initializeFromTileObject(const Blackguard::TileObject& object)
{
	TileObject obj = object;
	this->setPosition(sf::Vector2f(obj.x, obj.y));
	this->bounds.size = sf::Vector2f(obj.width, obj.height);
	if(obj.properties["orientation"] == "Left")
	{
		sf::IntRect rect=this->graphics.getTextureRect();
		rect.left  =  rect.width;
		rect.width = -rect.width;
		this->graphics.setTextureRect(rect);
	}
}

void Blackguard::BurglaryState::Exit::update(float deltaTime)
{
	if(enabled)
	{
		std::vector<Entity*> players = world->getEntitiesByType("Player");
		for(Entity* player : players)
			if(player->getBounds().intersects(this->bounds))
			{
				this->disable();
				world->onReachedExit();
			}
	}
}

void Blackguard::BurglaryState::Exit::updatePosition()
{
	Blackguard::BurglaryState::Entity::updatePosition();
	this->graphics.setPosition(position);
}

void Blackguard::BurglaryState::Exit::disable()
{
	enabled = false;
}

void Blackguard::BurglaryState::Exit::enable()
{
	enabled = true;
}
