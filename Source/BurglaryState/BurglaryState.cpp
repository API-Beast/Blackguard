/*
 *   Copyright 2013 Manuel Riecke <m.riecke@mrbeast.org>
 *   Copyright 2013 Raffael Zica <raffael@trollgames.de>
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

#include "BurglaryState.h"
#include "../EntityTypes.h"
#include "../Player.h"
#include "../Game.h"

using namespace Blackguard::BurglaryState;

BurglaryState::BurglaryState()
{
	map = new Map();
	map->add("playerEnt",EntityPtr(new Player(Game::instance->assets.textures["Player"])));
	tileMap.loadFromFile("test.tmx");
}

BurglaryState::~BurglaryState()
{
	delete map;
}

bool BurglaryState::processEvent(sf::Event& evt)
{
	map->processEvent(evt);
	return true;
}

void BurglaryState::update(float deltaTime)
{
	map->update(deltaTime);
}

void BurglaryState::draw(sf::RenderTarget* target)
{
	target->draw(tileMap);
	map->draw(target);
}
