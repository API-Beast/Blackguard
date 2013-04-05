#ifndef BLACKGUARD_PATHFINDER_H
#define BLACKGUARD_PATHFINDER_H

#include "TileMap.h"
#include <set>
#include <unordered_set>
#include <cstdint>
#include <stack>

namespace Blackguard
{
	struct PathNode
	{
		PathNode(int xTile, int yTile)
		{
			x = xTile;
			y = yTile;
			movementCost = -1;
			manhattanValue = -1;
			predecessor = -1;
		}

		PathNode() 
		{
			x = 0;
			y = 0;
			movementCost = -1;
			manhattanValue = -1;
			predecessor = -1;
		}

		PathNode(const PathNode& rhs)
		{
			x = rhs.x;
			y = rhs.y;
			movementCost = rhs.movementCost;
			manhattanValue = rhs.manhattanValue;
			predecessor = rhs.predecessor;
		}

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
		
		operator sf::Vector2f() const
		{
			return sf::Vector2f(x, y);
		}

		int x, y;
		int movementCost;
		int manhattanValue;
		int predecessor;
	};

	struct PathNodeHash {
		int64_t operator()(const PathNode &k) const
		{
			return uint64_t(k.y) << 8 + k.x;
		}
	};

	class PathFinder
	{
	public:
		PathFinder(TileMap& tileMap);
		~PathFinder() {}
		
		std::vector< sf::Vector2f > calculatePath(const sf::Vector2f& start, const sf::Vector2f& end, bool justInView=false);
	private:
		void expandNode(PathNode& currentNode);
		int calculateMoveCosts(const PathNode& current, const PathNode& successor);
		std::vector< sf::Vector2f > createWaypoints(const Blackguard::PathNode& endNode, bool justInView=false);
	private:
		TileMap& map;
		std::vector<PathNode> openList;
		std::unordered_set<PathNode,PathNodeHash> closedList;
		std::vector<PathNode> predecessorList;
		sf::Vector2i gridSize;
	};
}

#endif //BLACKGUARD_PATHFINDER_H