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
	return (successor.x - current.x != 0 && successor.y - current.y != 0) ? 10 : 14;
}

void PathFinder::expandNode(PathNode& currentNode)
{
	for(int y = -1; y < 2; y++)
	{
		for(int x = -1; x < 2; x++)
		{
			PathNode successor(currentNode.x + x, currentNode.y + y);

			if(x == 0 && y == 0)
				continue;

			if(!map.isBlockedByTile(currentNode.x + x, currentNode.y + y) && closedList.find(successor) == closedList.end()) {
				int movementCost = calculateMoveCosts(currentNode,successor);
			
				auto iter = std::find(openList.begin(), openList.end(), successor);
				if(iter != openList.end() && movementCost <= successor.movementCost)
					continue;

				if(successor.predecessor == -1)
				{
					successor.predecessor = predecessorList.size();
					predecessorList.push_back(currentNode);
				}
				else
					predecessorList[successor.predecessor] = currentNode;
				successor.movementCost = movementCost;

				
				if(iter == openList.end())
					openList.push_back(successor);
				else
					(*iter) = successor;
				
			}
		}
	}
}

std::vector<sf::Vector2f> PathFinder::createWaypoints(const PathNode& endNode)
{
	std::vector<sf::Vector2f> output;
	PathNode currentNode = endNode;
	while(currentNode.predecessor != -1)
	{
		output.push_back(sf::Vector2f(currentNode.x * gridSize.x + gridSize.x/2,currentNode.y * gridSize.y  + gridSize.y/2));
		currentNode = predecessorList[currentNode.predecessor];
	}
	return output;
}

std::vector<sf::Vector2f> PathFinder::calculatePath(const sf::Vector2f& start, const sf::Vector2f& end)
{
	gridSize = map.getGridSize();
	openList.clear();
	closedList.clear();
	predecessorList.clear();

	PathNode startNode(start.x / gridSize.x, start.y / gridSize.y);
	PathNode endNode  (end.x   / gridSize.x, end.y   / gridSize.y);
	if(map.isBlockedByTile(startNode.x, startNode.y)) printf("[DEBUG] Startpoint buried!\n");
	if(map.isBlockedByTile(endNode.x, endNode.y)) printf("[DEBUG] Endpoint buried!\n");
	
	openList.push_back(startNode);
	
	while(!openList.empty())
	{
		std::vector<PathNode>::iterator best=openList.begin();
		for(auto it = openList.begin(); it<openList.end(); it++)
		{
			if((*it) > (*best))
				best = it;
		}
		PathNode currentNode = *best;
		openList.erase(best);

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