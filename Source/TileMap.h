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

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <vector>
#include <map>
#include <exception>

namespace Blackguard
{

struct TileLayer
{
	TileLayer(){};
	unsigned int getTile(int x, int y) const;
	unsigned int& getTileRef(int x, int y);
	std::vector<unsigned int> data;
	int width, height;
};

struct TileObject
{
	enum Shape
	{
		Rectangle,
		Ellipse,
		Polygon,
		Polyline
	};
	
	std::string name;
	std::string type;
	int x, y;
	int width, height;
	float rotation;
	std::map<std::string, std::string> properties;
	Shape shape;
	// For the case of type == Polygon or Polyline
	std::vector<sf::Vector2i> points;
};

struct TileSet
{
	TileSet(){};
	unsigned int firstGID;
	unsigned int lastGID;
	int tileWidth, tileHeight;
	int spacing;
	sf::Texture texture;
	
	unsigned int amountTilesPerRow;
	float normalizedTileWidth, normalizedTileHeight;
	sf::Vector2f texCoordsForTile(unsigned int tileID) const;
};

class TileMap
{
public:
	class FormatNotSupported : public std::exception{};
public:
	void loadFromFile(const std::string& fileName);
	bool isBlocked(sf::Vector2f pos) const;
	bool isBlockedByTile(int x, int y) const;
	void markAsBlocked(sf::Vector2f pos);
	void markAsBlockedByTile(int x, int y);
	void unblock(sf::Vector2f pos);
	void unblockByTile(int x, int y);
	const std::vector<TileObject>& getObjects() const;
	sf::Vector2i getGridSize() const;
	void drawBackground(sf::RenderTarget* target, sf::RenderStates states=sf::RenderStates()) const;
	void drawForeground(sf::RenderTarget* target, sf::RenderStates states=sf::RenderStates()) const;
	void drawShadows(sf::RenderTarget* target, sf::RenderStates states=sf::RenderStates()) const;
protected:
	void drawLayers(const std::vector<TileLayer>& layers, sf::RenderTarget* target, sf::RenderStates states=sf::RenderStates()) const;
	void drawLayer(const Blackguard::TileLayer& layer, sf::RenderTarget* target, sf::RenderStates states, int minX, int minY, int maxX, int maxY) const;
private:
	std::vector<TileLayer> backgroundLayers;
	std::vector<TileLayer> foregroundLayers;
	std::vector<TileLayer> shadowLayers;
	TileLayer blockingLayer;
	std::vector<TileSet> tilesets;
	std::vector<TileObject> objects;
	int width, height;
	int gridWidth, gridHeight;
};

}

#endif // GRG_TILEMAP_H
