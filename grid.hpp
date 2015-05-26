#ifndef GRID
#define GRID


#include <vector>
#include "node.hpp"

class Grid
{
public:

	float nodeRadius = 16;
	float nodeDiameter;
	sf::Vector2f gridWorldSize;
	int gridSizeX;
	int gridSizeY;
	Node **grid;

	void Start();
	void createGrid();
	Grid();

	std::vector<Node> getNeighbours(Node node);
	Node nodeFromWorldPoint(sf::Vector2f WorldPos);
	


};



#endif