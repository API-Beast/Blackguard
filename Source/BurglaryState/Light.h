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

#ifndef BLACKGUARD_BURGLARYSTATE_LIGHT_H
#define BLACKGUARD_BURGLARYSTATE_LIGHT_H

#include "Entity.h"
#include <SFML/Graphics/Sprite.hpp>

namespace Blackguard
{
namespace BurglaryState
{

class Light : public Blackguard::BurglaryState::Entity
{
public:
	Light();
	virtual void drawLight(sf::RenderTarget* target, sf::RenderStates states=sf::RenderStates(sf::BlendAdd)) const;
	virtual std::string getType(){ return "Light"; };
	virtual void initializeFromTileObject(const Blackguard::TileObject& obj);
protected:
	virtual void updatePosition();
private:
	sf::Sprite graphics;
};
}
}

#endif // BLACKGUARD_BURGLARYSTATE_LIGHT_H
