#ifndef MAPGENERATOR
#define MAPGENERATOR

#define MAP_SIZE_X 50
#define MAP_SIZE_Y 38
#define CORRIDOR_CHANCE 10
#define ROOM_CHANCE 75

#include "random.hpp"

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
	MapGenerator();
	~MapGenerator();
	bool isInBounds(int x, int y) const;
	unsigned int checkNeighbourType(int x, int y, char tileType);
	int setTile(int x, int y, char tileType);
	int setTiles(int xStart, int yStart, int xEnd, int yEnd, char tileType);
	char getTile(int x, int y);
	int fillTiles(char tileType);
	int getRand(int min, int max);
	CardinalDirection getRandDirection();
	bool generateRoom(int x, int y, int xLength, int yLength, CardinalDirection direction);
	bool generateCorridor(int x, int y, int maxLength, CardinalDirection direction);
	bool eliminateDeadEnds();
	bool generateFeature();
	bool generateFeature(int x, int y, int xOffset, int yOffset, CardinalDirection direction);
	char** generateMap();
	bool isAreaType(int xStart, int yStart, int xEnd, int yEnd, char tileType);
	bool isAdjacent(int x, int y, char type);
	int writeMapFile(char(&mapArray)[MAP_SIZE_Y + 1][MAP_SIZE_Y + 1]);
private:
	int maxFeatures_;
	int maxTries_;
	int roomChance_;
	int corridorChance_;
	char **dungeonTiles_;
	Random *random;
};

#endif
