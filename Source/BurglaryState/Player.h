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

#ifndef GRG_PLAYER_H
#define GRG_PLAYER_H

#include <SFML/Graphics/Sprite.hpp>
#include "Entity.h"
#include "../Utility/Direction.h"

namespace sf
{
	class Event;
}

namespace Blackguard
{
	using Utility::Direction;
	namespace BurglaryState
	{
		class Player : public Entity
		{
		public:
			Player();
			virtual ~Player();

			// Entity stuff
			virtual void update(float deltaTime);
			virtual void draw(sf::RenderTarget* target) const;
			virtual void move(const sf::Vector2f& pos);
			virtual void setPosition(const sf::Vector2f& pos);
			virtual std::string getType(){ return "Player"; };
			
			// Controls
			bool activate();
			void setRunning(bool running);
			void setMoving(bool moving);
			void setMovingDirection(Direction dir);
			
			// State
			void addGold(int value);
			void setGold(int gold);
			int getGold() const;
		private:
			sf::Sprite graphics;
			bool isRunning;
			bool isMoving;
			Direction movingDir;
			int gold;
		};
	}
}

#endif //GRG_PLAYER_H