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

#include "Assets.h"
#include "Game.h"

#include <iostream>

using namespace Blackguard;

namespace
{
	#include <sys/stat.h>

	bool FileExists(const std::string& path)
	{
		struct stat fileInfo;
		int failed;
		failed = stat(path.c_str(), &fileInfo);
		return !failed && !(fileInfo.st_mode & S_IFDIR);
	}
}

Assets::Assets()
{
#ifdef BLACKGUARD_PREFER_INSTALLED_VERSION
	mDataPath = BLACKGUARD_INSTALL_PREFIX"/share/grg";
#else
	std::string paths[]= {"./Assets", BLACKGUARD_SOURCE_DIR"/Assets", BLACKGUARD_INSTALL_PREFIX"/share/grg"};

	for(const std::string& path : paths)
	{
		if(FileExists(path+"/AssetInfo.txt"))
		{
			this->path = path;
			break;
		}
	}

#endif
	std::cout << "Using data from " << path << std::endl;
}

void Assets::load()
{
	textures["Player"].loadFromFile(this->getPath("player.png"));
	textures["Stone" ].loadFromFile(this->getPath("stone.png" ));
	textures["Stone/Shadow"].loadFromFile(this->getPath("stoneShadow.png" ));
	textures["Noise" ].loadFromFile(this->getPath("Noise.png" ));
	textures["Loot"  ].loadFromFile(this->getPath("loot.png"  ));
	textures["Loot/Open"].loadFromFile(this->getPath("lootOpen.png"));
	textures["Guard"].loadFromFile(this->getPath("guard.png"));
	textures["Exit" ].loadFromFile(this->getPath("exit.png" ));
	textures["Light"].loadFromFile(this->getPath("light.png"));
	textures["Light/Ambient"].loadFromFile(this->getPath("lightAmbient.png"));
	textures["Light/Stripes"].loadFromFile(this->getPath("lightStripes.png"));
	textures["Light/Sun"    ].loadFromFile(this->getPath("lightSun.png"    ));
	textures["Light/Window" ].loadFromFile(this->getPath("lightWindow.png" ));
	textures["Light/Torch"  ].loadFromFile(this->getPath("lightTorch.png"  ));
	textures["Light/Smooth" ].loadFromFile(this->getPath("lightSmooth.png" ));
	// Tilesets
	textures["tilesetHouse.png"].loadFromFile(this->getPath("tilesetHouse.png"));
	textures["shadows.png"].loadFromFile(this->getPath("shadows.png"));

	sounds["stone_hit"].loadFromFile(this->getPath("stone_hit.ogg"));
	sounds["player_noticed"].loadFromFile(this->getPath("player_noticed.ogg"));
	sounds["walk"].loadFromFile(this->getPath("walk.ogg"));
	sounds["collect_loot"].loadFromFile(this->getPath("collect_loot.ogg"));
	sounds["game_over"].loadFromFile(this->getPath("game_over.ogg"));
	
	ambient.openFromFile(this->getPath("Music.ogg"));
}

std::string Assets::getPath(const std::string& path) const
{
	return this->path+"/"+path;
}
