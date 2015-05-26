#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <list>
#include "globals.hpp"
#include "map.hpp"
#include "node.hpp"
#include "grid.hpp"
#include "main.hpp"



Grid::Grid()
{
	Start();
}

void Grid::Start()
{
	nodeDiameter = nodeRadius * 2;
	gridSizeX = MAP_SIZE_X;
	gridSizeY = MAP_SIZE_Y;
	createGrid();
}

void Grid::createGrid()
{
	grid = new Node*[MAP_SIZE_X * MAP_SIZE_Y];

	for (int x = 0; x < gridSizeX; x++) {
		for (int y = 0; y < gridSizeY; y++) {
			sf::Vector2f worldPos((x * nodeDiameter + nodeRadius), (y * nodeDiameter + nodeRadius));
			bool walkable;
			if (game->map->gridCollision(x, y) == 1) {
				walkable = false;
			}
			else walkable = true;

			grid[x + MAP_SIZE_X * y] = new Node(walkable, worldPos, x, y);
		}
	}
}

std::vector<Node> Grid::getNeighbours(Node node)
{
	std::vector<Node> neighbours;

	for (int x = -1; x <= 1; x++) {
		for (int y = -1; y <= 1; y++) {
			if (x == 0 && y == 0) continue;

			int checkX = node.gridX + x;
			int checkY = node.gridY + y;

			if (checkX >= 0 && checkX < gridSizeX && checkY >= 0 && checkY < gridSizeY) {
				neighbours.push_back(*grid[checkX + MAP_SIZE_X * checkY]);
			}
		}
	}
	return neighbours;
}

Node Grid::nodeFromWorldPoint(sf::Vector2f worldPos)
{
	int x = worldPos.x / 32;
	int y = worldPos.y / 32;

	return *grid[x + MAP_SIZE_X * y];
}