#ifndef BLACKGUARD_PATHFINDER_H
#define BLACKGUARD_PATHFINDER_H

#include "TileMap.h"

namespace Blackguard
{
	struct PathNode
	{
		PathNode(int xTile, int yTile, PathNode* parentNode)
		{
			x = xTile;
			y = yTile;
			parent = parentNode;
		}

		PathNode() {}

		bool operator ==(const PathNode& other)
		{
			return x == other.x && y == other.y;
		}

		int x, y;
		int movementCost;
		int manhattanValue;
		PathNode* parent;
	};

	class PathFinder
	{
	public:
		PathFinder(TileMap* map);
		~PathFinder() {}
		
		void calculatePath(const sf::Vector2f& start, const sf::Vector2f& end);

	private:
		int calculateMoveCosts(const PathNode& node);
	private:
		TileMap* map;
	};
}

#endif //BLACKGUARD_PATHFINDER_H