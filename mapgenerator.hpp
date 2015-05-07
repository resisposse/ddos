/*
 * Dark Domains Of Space
 * 2015 © Project Team (see: LICENSE)
 */

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

class Point
{
public:
	Point() : x(NULL), y(NULL) {};
	Point(int x0, int y0) {
		int x = x0;
		int y = y0;
	};
	int x;
	int y;
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
	bool generateRoom(int x, int y, int width, int height, int wallWidth, CardinalDirection direction);
	bool generateCorridor(int x, int y, int width, int length, int wallWidth, CardinalDirection direction);
	bool eliminateDeadEnds();
	bool generateFeature();
	bool generateFeature(int x, int y, int xOffset, int yOffset, CardinalDirection direction);
	char* generateMap();
	bool isAreaType(int xStart, int yStart, int xEnd, int yEnd, char tileType);
	bool adjustPosition(int &x, int &y, int xStart, int yStart, int xEnd, int yEnd, char tileType, CardinalDirection direction);
	Point checkTiles(char tileType, int xStart, int yStart, int xEnd, int yEnd);
	bool isAdjacent(int x, int y, char type);
	int writeMapFile();
private:
	int maxFeatures_;
	int maxTries_;
	int roomChance_;
	int corridorChance_;
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
	int minCorridorWall_;
	int maxCorridorWall_;
	char *dungeonTiles_;
	Random *random;
};

#endif
