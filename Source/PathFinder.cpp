#include "PathFinder.h"
#include <functional>
#include <algorithm>
#include <queue>

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
	int itterations = 0;
	for(int y = -1; y < 2; y++)
	{
		for(int x = -1; x < 2; x++)
		{
			PathNode successor(currentNode.x + x, currentNode.y + y);

			if(x == 0 && y == 0)
				continue;

			itterations++;

			if(!map->isBlockedByTile(currentNode.x + x, currentNode.y + y) && closedList.find(successor) == closedList.end()) {
				int movementCost = calculateMoveCosts(currentNode,successor);
			
				if(openList.find(successor) != openList.end() && movementCost >= successor.movementCost) {
					continue;
				}

				successor.predecessor = &currentNode;
				successor.movementCost = movementCost;

				if(openList.find(successor) != openList.end()) {
					openList.erase(successor);
				}

				openList.insert(successor);
			}
		}
	}
}

void PathFinder::calculatePath(const sf::Vector2f& start, const sf::Vector2f& end)
{
	openList.clear();
	closedList.clear();

	PathNode startNode(start.x / gridSize.x, start.y / gridSize.y);
	PathNode endNode(end.x / gridSize.x, end.y / gridSize.y);
	
	openList.insert(startNode);
	
	while(!openList.empty())
	{
		PathNode currentNode = *openList.begin();
		openList.erase(openList.begin());

		if(currentNode == endNode)
			break;

		currentNode.manhattanValue = abs(endNode.x - currentNode.x) + abs(endNode.y - currentNode.y);
		expandNode(currentNode,endNode);
		closedList.insert(currentNode);
	}
}