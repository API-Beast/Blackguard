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

#include "Light.h"
#include "../Game.h"
#include "../TileMap.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include "../Utility/StringUtilities.h"

Blackguard::BurglaryState::Light::Light()
{
	graphics.setTexture(Game::instance->assets.textures["Light"]);
}

void Blackguard::BurglaryState::Light::drawLight(sf::RenderTarget* target, sf::RenderStates states) const
{
	target->draw(graphics, states);
}

void Blackguard::BurglaryState::Light::initializeFromTileObject(const Blackguard::TileObject& obj)
{
	this->setPosition(sf::Vector2f(obj.x, obj.y));
	graphics.setRotation(obj.rotation);
	if(obj.properties.find("strength") != obj.properties.end())
		graphics.setColor(sf::Color(255, 255, 255, Utility::FromString<int>(obj.properties.at("strength"))));
	if(obj.properties.find("variation") != obj.properties.end())
		graphics.setTexture(Game::instance->assets.textures["Light/"+obj.properties.at("variation")], true);
	graphics.setScale(obj.width/float(graphics.getTexture()->getSize().x), obj.height/float(graphics.getTexture()->getSize().y));
}

void Blackguard::BurglaryState::Light::updatePosition()
{
	Blackguard::BurglaryState::Entity::updatePosition();
	graphics.setPosition(position);
}