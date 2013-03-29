#include "PathFinder.h"
#include <functional>
#include <algorithm>
#include <queue>
#include <set>

using namespace Blackguard;

PathFinder::PathFinder(TileMap* map)
{
	this->map = map;
	gridSize = map->getGridSize();
}

int PathFinder::calculateMoveCosts(const PathNode& current, const PathNode& successor)
{
	return (successor.x - current.x != 0 && successor.y - current.y != 0) ? 14 : 10;
}

void PathFinder::expandNode(PathNode& currentNode, PathNode& endNode)
{
	for(int y = -1; y < 2; y++)
	{
		for(int x = -1; x < 2; x++)
		{
			PathNode successor(currentNode.x + x, currentNode.y + y);
			if(!map->isBlockedByTile(currentNode.x + x, currentNode.y + y) && closedList.find(successor) == closedList.end()) {
				currentNode.movementCost = calculateMoveCosts(currentNode,successor);
				currentNode.manhattanValue = abs(endNode.x - currentNode.x) + abs(endNode.y - currentNode.y);
				
			}
		}
	}
}

void PathFinder::calculatePath(const sf::Vector2f& start, const sf::Vector2f& end)
{
	openList = std::priority_queue<PathNode, std::vector<PathNode>, std::greater<PathNode>>();
	closedList.clear();

	PathNode startNode(start.x / gridSize.x, start.y / gridSize.y);
	PathNode endNode(end.x / gridSize.x, end.y / gridSize.y);
	
	openList.push(startNode);
	
	while(!openList.empty())
	{
		PathNode currentNode = openList.top();
		openList.pop();

		if(currentNode == endNode)
			break;

		expandNode(currentNode);

		closedList.insert(currentNode);
	}
}