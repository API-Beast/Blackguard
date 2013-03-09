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

#include "TileMap.h"
#include "Game.h"
#include "Dependencies/tinyxml2.h"
#include "Dependencies/base64.h"
#include <algorithm>

using namespace Blackguard;

void TileMap::draw(sf::RenderTarget& target, sf::RenderStates states)
{

}

void TileMap::loadFromFile(const std::string& fileName)
{
	using namespace tinyxml2;
	
	std::string fullPath = Game::instance->assets.getPath(fileName);
	XMLDocument file;
	file.LoadFile(fullPath.c_str());
	
	// Basic information about the map
  XMLElement* root=file.FirstChildElement("map");
  
	this->width  = root->IntAttribute("width" );
	this->height = root->IntAttribute("height");
	this->gridWidth  = root->IntAttribute("tilewidth" );
	this->gridHeight = root->IntAttribute("tileheight");
  
  // Parse them all!
  XMLElement* curElement = root->FirstChildElement();
	bool nextLayersAreForeground = false;
  do
  {
		std::string section = curElement->Value();
		if(section == "tileset")
		{
			TileSet tileset;
			
			tileset.firstGID   = curElement->IntAttribute("firstgid"  );
			tileset.tileWidth  = curElement->IntAttribute("tilewidth" );
			tileset.tileHeight = curElement->IntAttribute("tileheight");
			tileset.spacing    = curElement->IntAttribute("spacing"   );
			
			std::string imageSource = curElement->FirstChildElement("image")->Attribute("source");
			imageSource = Game::instance->assets.getPath(imageSource);
			tileset.texture.loadFromFile(imageSource);
			
			tilesets.push_back(std::move(tileset));
		}
		else if(section == "layer")
		{
			TileLayer layer;
			
			layer.width  = curElement->IntAttribute("width" );
			layer.height = curElement->IntAttribute("height");
			
			XMLElement* dataElement = curElement->FirstChildElement("data");
			std::string encoding    = dataElement->Attribute("encoding"   );
			std::string compression = dataElement->Attribute("compression");
			
			if(encoding != "base64")
				if(!compression.empty())
					throw FormatNotSupported();
				
			// The Data is stored as bytes in base64.
			std::string data = base64_decode(dataElement->GetText());
			layer.data.reserve(data.length() / 4);
			// Every ID is 4 bytes.
			for(int i=0; i < data.size(); i+=4)
				layer.data.push_back(
				            data[i + 0]       |
				            data[i + 1] << 8  |
				            data[i + 2] << 16 |
				            data[i + 3] << 24 );
			
			// There are a few layer names with special functionality.
			std::string name = curElement->Attribute("name");
			
			if(name == "Blocking")
				this->blockingLayer = std::move(layer);
			else
			{
				if(name == "Foreground")
					nextLayersAreForeground = true;
				
				if(nextLayersAreForeground)
				{
					foregroundLayers.push_back(std::move(layer));
				}
				else
				{
					backgroundLayers.push_back(std::move(layer));
				}
				
			}
			
		}
		else if(section == "objectgroup")
		{
			// TODO
		}
	}
	while(curElement=curElement->NextSiblingElement());
}
