/*
 * Dark Domains Of Space
 * 2015 © Project Team (see: LICENSE)
 */

#ifndef MAPGENERATOR
#define MAPGENERATOR

/* Forward declaration due to cyclic dependency */
class Random;

enum class CardinalDirection
{
	north = 0,
	east = 1,
	south = 2,
	west = 3,
	northEast = 4,
	southEast = 5,
	southWest = 6,
	northWest = 7
};

class MapGenerator
{
public:
	sf::Vector2f spawn;

	MapGenerator();
	~MapGenerator();
	void setTile(int x, int y, char tileType);
	void setTiles(int xStart, int yStart, int xEnd, int yEnd, char tileType);
	char getTile(int x, int y);
	void fillTiles(char tileType);
	void clearWalls();
	bool isInBounds(int x, int y) const;
	bool isInBounds(int x, int y, int offset, CardinalDirection direction) const;
	unsigned int checkNeighbourType(int x, int y, char tileType);
	bool isAreaType(int xStart, int yStart, int xEnd, int yEnd, char tileType);
	bool adjustPosition(int &x, int &y, int xStart, int yStart, int xEnd, int yEnd, char tileType, CardinalDirection direction);
	bool generateRoomCenter(int x, int y);
	bool generateTeleportRoom(int x, int y, int width, int height, int wallWidth, CardinalDirection direction);
	bool generateRoom(int x, int y, int width, int height, int wallWidth, char floorType, char wallType, CardinalDirection direction);
	bool generateCorridor(int x, int y, int width, int length, int wallWidth, char floorType, char wallType, CardinalDirection direction);
	bool generateFeature();
	bool generateFeature(int x, int y, CardinalDirection direction);
	char* generateMap();
	sf::Vector2i checkTiles(char tileType, int xStart, int yStart, int xEnd, int yEnd);
	int writeMapFile();
private:
	int maxFeatures_;
	int maxTries_;
	int roomChance_;
	int corridorChance_;
	int teleportRoomChance_;
	int minCorridorWidth_;
	int maxCorridorWidth_;
	int minCorridorLength_;
	int maxCorridorLength_;
	int minRoomWidth_;
	int maxRoomWidth_;
	int minRoomHeight_;
	int maxRoomHeight_;
	int minRoomWall_;
	int maxRoomWall_;
	int roomEntranceLength_;
	int minCorridorWall_;
	int maxCorridorWall_;
	char *dungeonTiles_;
	bool spawnCreated = false;
	bool goalCreated = false;
	unsigned int edgeOffset;
	Random *random;
};

#endif
