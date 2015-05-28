#ifndef PATHFINDING
#define PATHFINDING

class Object;

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


class Pathfinding
{
public:
	Grid *grid;
	Pathfinding();
	void Update();
	std::vector<sf::Vector2f> findPath(sf::Vector2f startPos, sf::Vector2f targetPos);
	std::vector<sf::Vector2f> retracePath(std::vector<Node> closedSet, Node targetNode);
	std::vector<sf::Vector2f> simplifyPath(std::vector<Node> path);
	int getDistance(Node nodeA, Node NodeB);
};

bool checkContains(std::vector<Node> Set, Node currentNode);


#endif