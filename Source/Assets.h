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

#ifndef GRG_ASSETS_H
#define GRG_ASSETS_H

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/Music.hpp>
#include <map>
#include "config.h"

namespace Blackguard
{
	class Assets
	{
	public:
		Assets();
		void load();
		std::string getPath(const std::string&) const;
	public:
		std::map<std::string, sf::Texture> textures;
		std::map<std::string, sf::SoundBuffer> sounds;
		sf::Music ambient;
		std::string path;
	};
}

#endif // GRG_ASSETS_H
