#ifndef BLACKGUARD_PATHFINDER_H
#define BLACKGUARD_PATHFINDER_H

#include "TileMap.h"

namespace Blackguard
{
	struct PathNode
	{
		PathNode(int xTile, int yTile)
		{
			x = xTile;
			y = yTile;
		}

		PathNode() {}

		bool operator ==(const PathNode& other) const
		{
			return x == other.x && y == other.y;
		}

		bool operator < (PathNode const& rhs) const
		{
			return getValue() < rhs.getValue();
		}

		bool operator > (PathNode const& rhs) const
		{
			return getValue() > rhs.getValue();
		}

		int getValue() const
		{
			return movementCost + manhattanValue;
		}

		int x, y;
		int movementCost;
		int manhattanValue;
	};

	class PathFinder
	{
	public:
		PathFinder(TileMap* map);
		~PathFinder() {}
		
		void calculatePath(const sf::Vector2f& start, const sf::Vector2f& end);
	private:
		void expandNode(PathNode& currentNode, PathNode& endNode);
		int calculateMoveCosts(const PathNode& current, const PathNode& successor);
	private:
		TileMap* map;
		std::priority_queue<PathNode, std::vector<PathNode>, std::greater<PathNode>> openList;
		std::set<PathNode> closedList;
		sf::Vector2i gridSize;
	};
}

#endif //BLACKGUARD_PATHFINDER_H