#include "PathFinder.h"

using namespace Blackguard;

PathFinder::PathFinder(TileMap* map)
{
	this->map = map;
}

int PathFinder::calculateMoveCosts(const PathNode& node)
{
	return (node.x - node.parent->x != 0 && node.y - node.parent->y != 0) ? 14 : 10;
}

void PathFinder::calculatePath(const sf::Vector2f& start, const sf::Vector2f& end)
{
	sf::Vector2i gridSize = map->getGridSize();
	std::vector<PathNode> open;
	std::vector<PathNode> closed;
	PathNode* selectedNode;

	// Sorry for German Comments here but I wanted to document that in a way that I will understand it 2 weeks later. (For now)

	/* Start Node hinzufügen und End Node erstellen */
	PathNode startNode(start.x / gridSize.x, start.y / gridSize.y, nullptr);
	PathNode endNode(end.x / gridSize.x, end.y / gridSize.y, nullptr);
	startNode.manhattanValue = 0;
	startNode.movementCost = 0;
	open.push_back(startNode);

	selectedNode = &startNode;

	while(std::find(closed.begin(), closed.end(), endNode) == closed.end())
	{
		for(auto node : open)
		{
			if(node.movementCost + node.manhattanValue < selectedNode->movementCost + selectedNode->manhattanValue) {
				selectedNode = &node;
			}
		}

		
		/* Begehbare Felder um den Start zur Open List hinzufügen */
		for(int y = -1; y < 2; y++)
		{
			for(int x = -1; x < 2; x++)
			{
				if(!map->isBlockedByTile(selectedNode->x + x, selectedNode->y + y)) {
					auto iterator = std::find(open.begin(), open.end(), PathNode(selectedNode->x + x, selectedNode->y + y, nullptr));
					if(iterator != open.end()) {
						PathNode node = *iterator;
						PathNode newNode = PathNode(selectedNode->x + x, selectedNode->y + y, selectedNode);
						newNode.movementCost = calculateMoveCosts(node);
						if(newNode.movementCost < node.movementCost) {
							node.movementCost = newNode.movementCost;
							node.parent = selectedNode;
							open.erase(iterator);
							open.push_back(node);
						}
					}
					else {
						PathNode node(selectedNode->x + x, selectedNode->y + y, selectedNode);
						node.movementCost = calculateMoveCosts(node);
						node.manhattanValue = abs(endNode.x - node.x) + abs(endNode.y - node.y);
						open.push_back(node);
					}
				}
			}
		}

		closed.push_back(*selectedNode);
		open.erase(std::remove(open.begin(), open.end(), *selectedNode), open.end());
	}

	system("pause");
}