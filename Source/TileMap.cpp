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
#include <SFML/Graphics.hpp>
#include <cmath>
#include <climits>
#include <GL/gl.h>
#include <sys/stat.h>

using namespace Blackguard;
using namespace std;

namespace
{
	
	template<typename T>
	struct GlRect
	{
		sf::Vector2<T> points[4];
		GlRect(T x, T y, T width, T height)
		{
			points[0].x=x;
			points[0].y=y;
			points[1].x=x+width;
			points[1].y=y;
			points[2].x=x+width;
			points[2].y=y+height;
			points[3].x=x;
			points[3].y=y+height;
		};
	};
}

int TileLayer::getTile(int x, int y) const
{
	if(x < 0 || y < 0 || x > width || y > height) return 0;
	return data[y*width + x];
}

sf::Vector2f TileSet::texCoordsForTile(int tileID) const
{
	int localID = tileID - this->firstGID;
	int x = localID % amountTilesPerRow;
	int y = localID / amountTilesPerRow;
	return sf::Vector2f(x*normalizedTileWidth, y*normalizedTileHeight);
}

void TileMap::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	sf::Vector2u viewportSize = target.getSize();
	sf::Vector2f leftTopCoordinate     = target.mapPixelToCoords(sf::Vector2i(0, 0));
	sf::Vector2f rightBottomCoordinate = target.mapPixelToCoords(sf::Vector2i(viewportSize));
	
	int minX = leftTopCoordinate.x / this->gridWidth;
	int minY = leftTopCoordinate.y / this->gridHeight;
	int maxX = rightBottomCoordinate.x / this->gridWidth;
	int maxY = rightBottomCoordinate.y / this->gridHeight;
	
	for(const TileLayer& layer : backgroundLayers)
	{
		for(int x=minX-1; x <= maxX; x++)
		{
			for(int y=minY-1; y <= maxY; y++)
			{
				// Global ID
				int tileGID = layer.getTile(x, y);
				if(tileGID == 0) continue; // Tile number 0 is "no-tile"
				
				// What tileset is that tile ID in?
				const TileSet* tileset;
				for(const TileSet& set : this->tilesets)
				{
					if(tileGID > set.firstGID && tileGID < set.lastGID)
					{
						tileset = &set;
						break;
					}
				}
				
				int xCoords, yCoords;
				xCoords = x*this->gridWidth;
				yCoords = y*this->gridHeight;
				
				sf::Vector2f tilePos = tileset->texCoordsForTile(tileGID);
				GlRect<GLfloat> vertices(xCoords, yCoords, tileset->tileWidth, tileset->tileHeight);
				GlRect<GLfloat> texCoords(tilePos.x, tilePos.y, tileset->normalizedTileWidth, tileset->normalizedTileHeight);
				
				sf::Vertex interleavedData[4];
				for(int i=0; i < 4; i++)
				{
					interleavedData[i].color = sf::Color::White;
					interleavedData[i].position  = vertices.points[i];
					interleavedData[i].texCoords = texCoords.points[i];
				}
				
				states.texture = &(tileset->texture);
				target.draw(interleavedData, 4, sf::Quads, states);
			}
		}
	}
}

void TileMap::loadFromFile(const std::string& fileName)
{
	using namespace tinyxml2;
	
	std::string fullPath = Game::instance->assets.getPath(fileName);
	XMLDocument file(true, COLLAPSE_WHITESPACE);
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
			tileset.lastGID    = INT_MAX;
			
			std::string imageSource = curElement->FirstChildElement("image")->Attribute("source");
			imageSource = Game::instance->assets.getPath(imageSource);
			tileset.texture.loadFromFile(imageSource);
			tileset.amountTilesPerRow = tileset.texture.getSize().x / tileset.tileWidth;
			//tileset.normalizedTileWidth  = tileset.tileWidth  / float(tileset.texture.getSize().x);
			//tileset.normalizedTileHeight = tileset.tileHeight / float(tileset.texture.getSize().y);
			// Geeeeeeez. SFML is using Rectangular Textures with unnormalized texture coordinates.
			tileset.normalizedTileWidth  = tileset.tileWidth;
			tileset.normalizedTileHeight = tileset.tileHeight;
			
			if(!tilesets.empty())
				tilesets.back().lastGID = tileset.firstGID - 1;
			tilesets.push_back(std::move(tileset));
		}
		else if(section == "layer")
		{
			TileLayer layer;
			
			layer.width  = curElement->IntAttribute("width" );
			layer.height = curElement->IntAttribute("height");
			
			XMLElement* dataElement = curElement->FirstChildElement("data");
			std::string encoding    = dataElement->Attribute("encoding"   );
			
			if(encoding != "base64" || dataElement->Attribute("compression"))
				throw FormatNotSupported();
				
			// The Data is stored as bytes in base64.
			std::string encodedData = dataElement->GetText();
			std::string data = base64_decode(encodedData);
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


bool TileMap::isBlocked(sf::Vector2i pos)
{
	return blockingLayer.getTile(pos.x, pos.y);
}

