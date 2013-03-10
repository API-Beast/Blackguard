/*
 *   Copyright (C) 2013 Manuel Riecke <m.riecke@mrbeast.org>
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

#ifndef GRG_GAME_H
#define GRG_GAME_H

#include <SFML/System/Clock.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <string>

#include "Assets.h"
#include "GameData.h"

namespace sf
{
	class RenderWindow;
}

namespace Blackguard
{
	class GameState;
	namespace BurglaryState { class BurglaryState; }
	namespace WorldmapState { class WorldmapState; }

	class Game
	{
	public:
		Game(int argc, char** argv);
		virtual ~Game();
		int run();
		float getDeltaTime() const;
		void draw();
		void update();
		void processEvents();
	public:
		static Game* instance;
		Assets assets;
		GameData data;
		GameState* currentGameState;

		struct // States
		{
			BurglaryState::BurglaryState* burglary;
			WorldmapState::WorldmapState* worldmap;
		} State;
	private:
		sf::RenderWindow* window;
		sf::Clock clock;
		float deltaTime;
	};
}

#endif // GRG_GAME_H
