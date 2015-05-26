#ifndef NODE
#define NODE

class Node
{
public:
	bool walkable;
	sf::Vector2f worldPos;
	int gridX;
	int gridY;
	int gCost;
	int hCost;
	sf::Vector2f parentCoords;
	//std::vector<Node> parent;
	Node *parent;

	Node();
	Node(bool _walkable, sf::Vector2f _worldPos, int _gridX, int _gridY);
	int fCost();
	//sf::Vector2f parent();
	bool operator==(const Node &other) const;
	bool operator!=(const Node &other) const;
};

#endif