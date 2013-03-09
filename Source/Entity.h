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

#ifndef GRG_ENTITY_H
#define GRG_ENTITY_H

#include "SFML/System.hpp"
#include "SFML/Window.hpp"

namespace Blackguard
{
	class Entity
	{
	public:
		virtual void update(float deltaTime) = 0;
		virtual void draw() = 0;
		virtual bool processEvent(sf::Event& evt) = 0;

		// Setter:
		virtual void move(sf::Vector2f pos) { position += pos; }
		virtual void setPosition(sf::Vector2f pos) { position = pos; }

		// Getter:
		virtual sf::Vector2f getPosition() const { return position; }
		virtual sf::Vector2i getSize() const { return size; }
	protected:
		sf::Vector2f position;
		sf::Vector2i size;
	};
}

#endif //GRG_ENTITY_H