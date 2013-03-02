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

using namespace GRG;

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
#ifdef GRG_PREFER_INSTALLED_VERSION
  mDataPath = GRG_INSTALL_PREFIX"/usr/share/grg";  
#else
	std::string paths[]={"./Assets", GRG_SOURCE_DIR"/Assets", GRG_INSTALL_PREFIX"/share/grg"};
  /*std::vector<std::string> paths;
  paths.push_back("./Assets");
  paths.push_back(GRG_SOURCE_DIR"/Assets");
  paths.push_back(GRG_INSTALL_PREFIX"/share/grg");*/
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
  textures["TitleTest"].loadFromFile(this->getPath("TitleTest.png"));
}

std::string Assets::getPath(const std::string& path) const
{
  return this->path+"/"+path;
}
