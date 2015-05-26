#ifndef PATHFINDING
#define PATHFINDING

class Object;

class Pathfinding
{
public:
	Grid *grid;
	Pathfinding();
	void Awake();
	void Update();
	std::vector<sf::Vector2f> findPath(sf::Vector2f startPos, sf::Vector2f targetPos);
	std::vector<sf::Vector2f> retracePath(std::vector<Node> closedSet, Node targetNode);
	std::vector<sf::Vector2f> simplifyPath(std::vector<Node> path);
	int getDistance(Node nodeA, Node NodeB);

	
};

bool checkContains(std::vector<Node> Set, Node currentNode);

#endif