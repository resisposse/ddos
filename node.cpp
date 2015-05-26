#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include "node.hpp"


Node::Node(bool _walkable, sf::Vector2f _worldPos, int _gridX, int _gridY)
{
	walkable = _walkable;
	worldPos = _worldPos;
	gridX = _gridX;
	gCost = 0;
	hCost = 0;
	gridY = _gridY;
}

Node::Node()
{
	gridX = 0;
	gridY = 0;
	gCost = 0;
	hCost = 0;
}

int Node::fCost()
{
		return gCost + hCost;
}
/*
sf::Vector2f Node::parent() {
	return parentCoords;
}
*/

bool Node::operator==(const Node &other) const
{
	return (*this == other);
}

bool Node::operator!=(const Node &other) const
{
	return !(*this == other);
}



