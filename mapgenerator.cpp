/*
 * Dark Domains Of Space
 * 2015 © Project Team (see: LICENSE)
 */

#include <iostream>
#include <fstream>
#include "mapgenerator.hpp"

/*
Corridor and Room parameters except walls have to be 1 less than what you want it to be due to North/West bug
Currently the program will be stuck in an infinite loop if corridor width is more than 1
*/
MapGenerator::MapGenerator()
{
	maxFeatures_ = 1000;
	maxTries_ = 3000;
	roomChance_ = 75;
	corridorChance_ = 25;
	minCorridorWidth_ = 1;
	maxCorridorWidth_ = 1;
	minCorridorLength_ = 3;
	maxCorridorLength_ = 8;
	minRoomWidth_ = 4;
	maxRoomWidth_ = 8;
	minRoomHeight_ = 4;
	maxRoomHeight_ = 8;
	minRoomWall_ = 1;
	maxRoomWall_ = 1;
	minCorridorWall_ = 3;
	maxCorridorWall_ = 3;

	//random = new Random(-554451646);
	//random = new Random(630474362);
	//random = new Random(-324032844);
	//random = new Random(4168282);
	//random = new Random(-367358048);
	//random = new Random(1119802934);
	//random = new Random(967450586);
	random = new Random();

	dungeonTiles_ = new char[MAP_SIZE_X * MAP_SIZE_Y];
}

MapGenerator::~MapGenerator()
{
	delete random;
	delete [] dungeonTiles_;
}

int MapGenerator::setTile(int x, int y, char tileType)
{
	dungeonTiles_[x + MAP_SIZE_X * y] = tileType;
	return 0;
}

/*This function contains a workaround for the north/west bug which makes all features 1 tile longer in each axis*/
int MapGenerator::setTiles(int xStart, int yStart, int xEnd, int yEnd, char tileType)
{
	for (int i = xStart; i != xEnd + 1; ++i) {
		for (int j = yStart; j != yEnd + 1; ++j) {
			dungeonTiles_[i + MAP_SIZE_X * j] = tileType;
		}
	}
	return 0;
}

int MapGenerator::fillTiles(char tileType)
{
	for (int i = 0; i < MAP_SIZE_X; i++) {
		for (int j = 0; j < MAP_SIZE_Y; j++) {
			dungeonTiles_[i + MAP_SIZE_X * j] = tileType;
		}
	}
	return 0;
}

char MapGenerator::getTile(int x, int y)
{
	return dungeonTiles_[x + MAP_SIZE_X * y];
}

CardinalDirection MapGenerator::getRandDirection()
{
	return CardinalDirection(random->generate(0, 3));
}

bool MapGenerator::isInBounds(int x, int y) const
{
	return x >= 2 && x < MAP_SIZE_X - 2 && y >= 2 && y < MAP_SIZE_Y - 2;
}

bool MapGenerator::isAreaType(int xStart, int yStart, int xEnd, int yEnd, char tileType)
{
	if (!isInBounds(xStart, yStart)) {
		return false;
	}
	else if (!isInBounds(xEnd, yEnd)) {
		return false;
	}

	for (int i = xStart; i < xEnd + 1; i++) {
		for (int j = yStart; j < yEnd + 1; j++) {
			if (dungeonTiles_[i + MAP_SIZE_X * j] != tileType) {
				//std::cout << i << "," << j << " is occupied" << std::endl;
				return false;
			}
		}
	}
	return true;
}

bool MapGenerator::isAdjacent(int x, int y, char tileType)
{
	if (getTile(x, y - 1) == tileType ||
		getTile(x - 1, y) == tileType ||
		getTile(x + 1, y) == tileType ||
		getTile(x, y + 1) == tileType)
		return true;
	else
		return false;
}

unsigned int MapGenerator::checkNeighbourType(int x, int y, char tileType)
{
	unsigned int neighbours = 0;

	if (getTile(x - 1, y - 1) == tileType) { neighbours += 128; };
	if (getTile(x, y - 1    ) == tileType) { neighbours += 64; };
	if (getTile(x + 1, y - 1) == tileType) { neighbours += 32; };
	if (getTile(x - 1, y    ) == tileType) { neighbours += 16; };
	if (getTile(x + 1, y    ) == tileType) { neighbours += 8; };
	if (getTile(x - 1, y + 1) == tileType) { neighbours += 4; };
	if (getTile(x, y + 1    ) == tileType) { neighbours += 2; };
	if (getTile(x + 1, y + 1) == tileType) { neighbours += 1; };
	return neighbours;
}

bool MapGenerator::adjustPosition(int &x, int &y, int xStart, int yStart, int xEnd, int yEnd, char tileType, CardinalDirection direction) 
{
	Point point = checkTiles(tileType, xStart, yStart, xEnd, yEnd);
	while (point.x != NULL) {
		//std::cout << "adjusting coordinates" << std::endl;
		if (direction == CardinalDirection::north || direction == CardinalDirection::south) {
			if (point.x > x) {
				x--;
				xStart--;
				xEnd--;
				std::cout << "moved position west" << std::endl;
			}
			else if (point.x < x)  {
				x++;
				xStart++;
				xEnd++;
				std::cout << "moved position east" << std::endl;
			}
			else {
				std::cout << "could not adjust position" << std::endl;
				return false;
			}
		}
		else if (direction == CardinalDirection::east || direction == CardinalDirection::west) {
			if (point.y > y) {
				y--;
				yStart--;
				yEnd--;
				std::cout << "moved position north" << std::endl;
			}
			else if (point.y < y)  {
				y++;
				yStart++;
				yEnd++;
				std::cout << "moved position south" << std::endl;
			}
			else {
				std::cout << "could not adjust position" << std::endl;
				return false;
			}
		}
		point = checkTiles(tileType, xStart, yStart, xEnd, yEnd);
	}
	return true;
}

Point MapGenerator::checkTiles(char tileType, int xStart, int yStart, int xEnd, int yEnd)
{
	Point coords;
	if (!isInBounds(xStart, yStart) || !isInBounds(xEnd, yEnd)) {
		//std::cout << "out of bounds" << std::endl;
		return coords;
	}

	for (int i = xStart; i < xEnd + 1; i++) {
		for (int j = yStart; j < yEnd + 1; j++) {
			if (getTile(i, j) != tileType) {
				//std::cout << i << "," << j << " is occupied2" << std::endl;
				coords.x = i;
				coords.y = j;
				std::cout << "coords: [" << coords.x << "," << coords.y << "]" << std::endl;
				return coords;
			}
		}
	}
	return coords;
}

/*
 * This function will create a room based on the refence coordinates, x and y, as well as the direction.
 * It will first find the starting and ending coordinates to be passed for the setTiles function
 */
bool MapGenerator::generateRoom(int x, int y, int width, int height, int wallWidth, CardinalDirection direction)
{
	int xStart = x;
	int yStart = y;

	int xEnd = x;
	int yEnd = y;

	if (direction == CardinalDirection::north) {
		xStart = x - width / 2;
		yStart = y - height;
		xEnd = x + (width + 1) / 2;
	} else if (direction == CardinalDirection::east) {
		yStart = y - height / 2;
		xEnd = x + width;
		yEnd = y + (height + 1) / 2;
	} else if (direction == CardinalDirection::south) {
		xStart = x - width / 2;
		xEnd = x + (width + 1) / 2;
		yEnd = y + height;
	} else if (direction == CardinalDirection::west) {
		xStart = x - width;
		yStart = y - height / 2;
		yEnd = y + (height + 1) / 2;
	} else {
		std::cout << "Could not set coords for room creation" << std::endl;
	}

	if (!isAreaType(xStart, yStart, xEnd + 1, yEnd + 1, 'X')) {
		//std::cout << "Could not create room" << std::endl;
		return false;
	}

	setTiles(xStart, yStart, xEnd, yEnd, 'X');
	setTiles(xStart + wallWidth, yStart + wallWidth, xEnd - wallWidth, yEnd - wallWidth, '.');

	//std::cout << "Room created between [" << xStart + wallWidth << "," << yStart + wallWidth << "] and [" << xEnd - wallWidth << "," << yEnd - wallWidth << "]." << std::endl;
	//std::cout << "  Width: " << width << ", height: " << height << ", wall width: " << wallWidth << "." << std::endl;
	return true;
}

bool MapGenerator::generateCorridor(int x, int y, int width, int length, int wallWidth, CardinalDirection direction)
{

	//std::cout << "Starting corridor creation" << std::endl;
	int offset = wallWidth;

	int xStart = x;
	int yStart = y;
	int xEnd = x;
	int yEnd = y;

	int xStartOffset = 0;
	int yStartOffset = 0;
	int xEndOffset = 0;
	int yEndOffset = 0;

	if (direction == CardinalDirection::north) {
		adjustPosition(x, y, x - width / 2, y + 1, x + (width + 1) / 2, y + 1, '.', CardinalDirection::north);
		xStart = x - width / 2;
		yStart = y - length;
		xEnd = x + (width + 1) / 2;
		xStartOffset = offset;
		xEndOffset = offset;
	} else if (direction == CardinalDirection::east) {
		adjustPosition(x, y, x - 1, y - width / 2, x - 1, y + (width + 1) / 2, '.', CardinalDirection::east);;
		yStart = y - width / 2;
		xEnd = x + length;
		yEnd = y + (width + 1) / 2;
		yStartOffset = offset;
		yEndOffset = offset;
	} else if (direction == CardinalDirection::south) {
		adjustPosition(x, y, x - width / 2, y - 1, x + (width + 1) / 2, y - 1, '.', CardinalDirection::south);
		xStart = x - width / 2;
		xEnd = x + (width + 1) / 2;
		yEnd = y + length;
		xStartOffset = offset;
		xEndOffset = offset;
	} else if (direction == CardinalDirection::west) {
		adjustPosition(x, y, x + 1, y - width / 2, x + 1, y + (width + 1) / 2, '.', CardinalDirection::west);
		xStart = x - length;
		yStart = y - width / 2;
		yEnd = y + (width + 1) / 2;
		yStartOffset = offset;
		yEndOffset = offset;
	}

	if (!isAreaType(xStart - xStartOffset, yStart - yStartOffset, xEnd + xEndOffset, yEnd + yEndOffset, 'X')) {
		//setTile(x, y, '0');
		//std::cout << "Could not create corridor." << std::endl;
		return false;
	}

	setTiles(xStart, yStart, xEnd, yEnd, '.');
	//std::cout << "Corridor created between [" << xStart << "," << yStart << "] and [" << xEnd << "," << yEnd << "]." << std::endl; 
	//std::cout << "  Width: " << width << ", Length: " << length << "." << std::endl;
	return true;
}

/*
 * Due to a bug, all features facing north or west are drawn 1 tile to far from the target, this has been patched with a workaround in setTiles function
 * 
 * This function will first pick a random wall tile that is adjacent to a room or corridor.
 * Then it will create a feature facing either north, east, south or west, depending on which side of the room or corridor that wall is.
 */
bool MapGenerator::generateFeature()
{
	int x = 0;
	int y = 0;

	for (int tries = 0; tries != maxTries_; ++tries) {
		x = random->generate(1, MAP_SIZE_X - 1);
		y = random->generate(1, MAP_SIZE_Y - 1);

		if (getTile(x,y) != 'X') {
			continue;
		}

		if (getTile(x, y + 1) == '.') {
			//std::cout << "Generating feature facing north at [" << x << "," << y << "]" << std::endl;
			if (generateFeature(x, y, 0, -1, CardinalDirection::north)) {
				return true;
			}
		}
		else if (getTile(x - 1, y) == '.') {
			//std::cout << "Generating feature facing east at [" << x << "," << y << "]" << std::endl;
			if (generateFeature(x, y, 1, 0, CardinalDirection::east)) {
				return true;
			}
		}
		else if (getTile(x, y - 1) == '.') {
			//std::cout << "Generating feature facing south at [" << x << "," << y << "]" << std::endl;
			if (generateFeature(x, y, 0, 1, CardinalDirection::south)) {
				return true;
			}
		}
		else if (getTile(x + 1, y) == '.') {
			//std::cout << "Generating feature facing west at [" << x << "," << y << "]" << std::endl;
			if (generateFeature(x, y, -1, 0, CardinalDirection::west)) {
				return true;
			}
		}
	}
	return false;
}


/*
 * This function will roll a number and create either a room or corridor based on the outcome
 * The chance for the feature can be adjusted in the constructor
 */
bool MapGenerator::generateFeature(int x, int y, int xOffset, int yOffset, CardinalDirection direction)
{
	int roll = random->generate(0, 100);

	if (roll <= corridorChance_) {
		int corridorWall = random->generate(minCorridorWall_, maxCorridorWall_);
		if (generateCorridor(x, y, random->generate(minCorridorWidth_, maxCorridorWidth_), random->generate(minCorridorLength_, maxCorridorLength_), corridorWall, direction)) {
				return true;
			}
		return false;
	} else {
		int roomWall = random->generate(minRoomWall_, maxRoomWall_);
		if (generateRoom(x + xOffset, y + yOffset, random->generate(minRoomWidth_, maxRoomWidth_), random->generate(maxRoomHeight_, maxRoomHeight_), roomWall, direction)) {
			(generateCorridor(x, y, random->generate(minCorridorWidth_, maxCorridorWidth_), roomWall, 1, direction));
			return true;
		}
		return false;
	}
}


char* MapGenerator::generateMap()
{
	fillTiles('X');
	generateRoom(MAP_SIZE_X / 2, MAP_SIZE_Y / 2, random->generate(minRoomWidth_, maxRoomWidth_), random->generate(maxRoomHeight_, maxRoomHeight_), random->generate(minRoomWall_, maxRoomWall_), getRandDirection());
	
	/*for (int i = 0; i < 1; i++) {
		if (!generateFeature()) {
			break;
		}
	}*/

	for (int features = 1; features != maxFeatures_; features++) {
		if (!generateFeature()) {
			break;
		}
	}

	std::cout << "Seed: " << random->getSeed() << std::endl;
	return dungeonTiles_;
}


int MapGenerator::writeMapFile()
{
	std::ofstream out;
	out.open("map.dat");
	for (int i = 0; i < MAP_SIZE_X; i++) {
		for (int j = 0; j < MAP_SIZE_Y + 1; j++) {
			out.put(dungeonTiles_[i + MAP_SIZE_X * j]);
		}
	}
	out.close();
	return 0;
}
