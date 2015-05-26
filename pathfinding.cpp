#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
#include <cmath>
#include "object.hpp"
#include "node.hpp"
#include "grid.hpp"
#include "pathfinding.hpp"
#include "main.hpp"

Pathfinding::Pathfinding()
{
	grid = new Grid();
}

void Pathfinding::Awake()
{
	grid = new Grid();
}

void Pathfinding::Update()
{
	//findPath()
}

std::vector<sf::Vector2f> Pathfinding::findPath(sf::Vector2f startPos, sf::Vector2f targetPos)
{


	Node startNode = grid->nodeFromWorldPoint(startPos);
	Node targetNode = grid->nodeFromWorldPoint(targetPos);


	if (startNode.walkable && targetNode.walkable) {

		std::vector<Node> openSet;
		std::vector<Node> closedSet;
		openSet.push_back(startNode);

		while (openSet.size() > 0) {
			Node currentNode = openSet[0];
			int temp = 0;
			for (int i = 1; i < openSet.size(); i++) {
				if (openSet[i].fCost() < currentNode.fCost() || openSet[i].fCost() == currentNode.fCost() && openSet[i].hCost < currentNode.hCost) {
					currentNode = openSet[i];
					temp = i;
				}
			}

			openSet.erase(openSet.begin() + temp);
			closedSet.push_back(currentNode);


			if (currentNode.gridX == targetNode.gridX && currentNode.gridY == targetNode.gridY) {
				std::vector<sf::Vector2f> waypoints(retracePath(closedSet, targetNode));
				return waypoints;
			}

			for each (Node neighbour in grid->getNeighbours(currentNode)) {
				if (!neighbour.walkable || checkContains(closedSet, neighbour)) {
					continue;
				}

				int newMovementCostToNeighbour = currentNode.gCost + ((neighbour.gridX - currentNode.gridX == 0 || neighbour.gridY - currentNode.gridY == 0) ? 10 : 14);
				if (newMovementCostToNeighbour < neighbour.gCost || !checkContains(openSet, neighbour)) {
					neighbour.gCost = newMovementCostToNeighbour;
					neighbour.hCost = getDistance(neighbour, targetNode);
					neighbour.parentCoords = currentNode.worldPos;

					if (!checkContains(openSet, neighbour))
						openSet.push_back(neighbour);
				}
			}

		}
	}
}

std::vector<sf::Vector2f> Pathfinding::retracePath(std::vector<Node> closedSet, Node targetNode)
{
	std::vector<Node> path;

	Node currentNode = closedSet[closedSet.size() - 1];

	//path2.push_back(targetNode.worldPos);

	for (int i = closedSet.size() -1 ; i >= 0; i--) {

		if (closedSet[i].worldPos == currentNode.parentCoords) {
			path.push_back(closedSet[i]);
			currentNode = closedSet[i];
		}
	}
	std::vector<sf::Vector2f> waypoints = simplifyPath(path);

	return waypoints;
}

std::vector<sf::Vector2f> Pathfinding::simplifyPath(std::vector<Node> path)
{
	std::vector<sf::Vector2f> waypoints;
	waypoints.push_back(path[0].worldPos);
	sf::Vector2f directionOld(0, 0);
	for (int i = 1; i < path.size(); i++) {
		sf::Vector2f directionNew(path[i - 1].gridX - path[i].gridX, path[i - 1].gridY - path[i].gridY);
		if (directionNew != directionOld) {
			waypoints.push_back(path[i - 1].worldPos);
			waypoints.push_back(path[i].worldPos);
		}
		directionOld = directionNew;
	}
	waypoints.push_back(path[path.size() -1].worldPos);
	return waypoints;
}

int Pathfinding::getDistance(Node nodeA, Node nodeB)
{
	int distX = abs(nodeA.gridX - nodeB.gridX);
	int distY = abs(nodeA.gridY - nodeB.gridY);

	if (distX > distY)
		return 14 * distY + 10 * (distX - distY);
	return 14 * distX + 10 * (distY - distX);
}

bool checkContains(std::vector<Node> Set, Node currentNode)
{
	bool contains = false;

	for each (Node node in Set) {
		if (node.gridX == currentNode.gridX && node.gridY == currentNode.gridY) {
			contains = true;
			break;
		}
	}
	return contains;
}