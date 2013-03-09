/*
 *   Copyright 2013 Manuel Riecke <m.riecke@mrbeast.org>
 *   Copyright 2013 Raffael Zica <raffael@trollgames.de>
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

#ifndef GRG_TILEMAP_H
#define GRG_TILEMAP_H

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Text.hpp>
#include <map>
#include <exception>

namespace Blackguard
{

struct TileLayer
{
	std::vector<int> data;
	int width, height;
};

struct TileSet
{
	int firstGID;
	int tileWidth, tileHeight;
	int spacing;
	sf::Texture texture;
// struct TileProperties
// {
// };
// std::map<int, TileProperties> tileProperties;
};

class TileMap : public sf::Drawable
{
public:
	class FormatNotSupported : public std::exception{};
public:
	void loadFromFile(const std::string& fileName);
protected:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states);
private:
	std::vector<TileLayer> backgroundLayers;
	std::vector<TileLayer> foregroundLayers;
	TileLayer blockingLayer;
	std::vector<TileSet> tilesets;
	int width, height;
	int gridWidth, gridHeight;
};

}

#endif // GRG_TILEMAP_H
