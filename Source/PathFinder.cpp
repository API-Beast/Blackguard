#include "PathFinder.h"
#include <functional>
#include <algorithm>
#include <queue>

using namespace Blackguard;

PathFinder::PathFinder(TileMap& tileMap) : map(tileMap)
{

}

int PathFinder::calculateMoveCosts(const PathNode& current, const PathNode& successor)
{
	return (successor.x - current.x != 0 && successor.y - current.y != 0) ? 14 : 10;
}

void PathFinder::expandNode(PathNode& currentNode)
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

			if(!map.isBlockedByTile(currentNode.x + x, currentNode.y + y) && closedList.find(successor) == closedList.end()) {
				int movementCost = calculateMoveCosts(currentNode,successor);
			
				if(openList.find(successor) != openList.end() && movementCost >= successor.movementCost) {
					continue;
				}

				if(successor.predecessor == -1)
				{
					successor.predecessor = predecessorList.size();
					predecessorList.push_back(currentNode);
				}
				else
					predecessorList[successor.predecessor] = currentNode;
				successor.movementCost = movementCost;

				if(openList.find(successor) != openList.end()) {
					openList.erase(successor);
				}

				openList.insert(successor);
			}
		}
	}
}

std::stack<sf::Vector2f> PathFinder::createWaypoints(const PathNode& endNode)
{
	std::stack<sf::Vector2f> output;
	PathNode currentNode = endNode;
	while(currentNode.predecessor != -1)
	{
		output.push(sf::Vector2f(currentNode.x * gridSize.x,currentNode.y * gridSize.y));
		currentNode = predecessorList[currentNode.predecessor];
	}
	return output;
}

std::stack<sf::Vector2f> PathFinder::calculatePath(const sf::Vector2f& start, const sf::Vector2f& end)
{
	gridSize = map.getGridSize();
	openList.clear();
	closedList.clear();
	predecessorList.clear();

	PathNode startNode(start.x / gridSize.x, start.y / gridSize.y);
	PathNode endNode(end.x / gridSize.x, end.y / gridSize.y);
	
	openList.insert(startNode);
	
	while(!openList.empty())
	{
		PathNode currentNode = *openList.begin();
		openList.erase(openList.begin());

		if(currentNode == endNode) {
			endNode.predecessor = currentNode.predecessor; // copy the correct predecessor
			break;
		}

		currentNode.manhattanValue = abs(endNode.x - currentNode.x) + abs(endNode.y - currentNode.y);
		expandNode(currentNode);
		closedList.insert(currentNode);
	}

	if(openList.empty())
		printf("[DEBUG] No way found to Path!\n");

	return createWaypoints(endNode);
}