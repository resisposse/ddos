/*
 * Dark Domains Of Space
 * 2015 © Project Team (see: LICENSE)
 */

#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include "globals.hpp"
#include "random.hpp"
#include "mapgenerator.hpp"

/*
 * Corridor and Room parameters except walls have to be 1 less than what you
 * want it to be due to North/West bug. Currently the program will be stuck in
 * an infinite loop if corridor width is more than 1
 */
MapGenerator::MapGenerator()
{
	maxFeatures_ = 1000;
	maxTries_ = 3000;
	roomChance_ = 74;
	corridorChance_ = 25;
	teleportRoomChance_ = 1;
	minCorridorWidth_ = 1;
	maxCorridorWidth_ = 1;
	minCorridorLength_ = 4;
	maxCorridorLength_ = 7;
	minRoomWidth_ = 3;
	maxRoomWidth_ = 6;
	minRoomHeight_ = 3;
	maxRoomHeight_ = 6;
	minRoomWall_ = 2;
	maxRoomWall_ = 2;
	roomEntranceLength_ = 0;
	minCorridorWall_ = 3;
	maxCorridorWall_ = 3;

	edgeOffset = 15;

	random = new Random();

	dungeonTiles_ = new char[MAP_SIZE_X * MAP_SIZE_Y];
}

MapGenerator::~MapGenerator()
{
	delete random;
	delete [] dungeonTiles_;
}

void MapGenerator::setTile(int x, int y, char tileType)
{
	dungeonTiles_[x + MAP_SIZE_X * y] = tileType;
}

/*
 * This function contains a workaround for the north/west bug which makes all
 * features 1 tile longer in each axis
 */
void MapGenerator::setTiles(int xStart, int yStart, int xEnd, int yEnd, char tileType)
{
	for (int i = xStart; i != xEnd + 1; ++i) {
		for (int j = yStart; j != yEnd + 1; ++j) {
			dungeonTiles_[i + MAP_SIZE_X * j] = tileType;
		}
	}
}

void MapGenerator::fillTiles(char tileType)
{
	for (int i = 0; i < MAP_SIZE_X; i++) {
		for (int j = 0; j < MAP_SIZE_Y; j++) {
			dungeonTiles_[i + MAP_SIZE_X * j] = tileType;
		}
	}
}

char MapGenerator::getTile(int x, int y)
{
	return dungeonTiles_[x + MAP_SIZE_X * y];
}

sf::Vector2f getRoomCenter(int x, int y, int roomWidth, int roomHeight, CardinalDirection direction)
{
	sf::Vector2f center(x, y);
	if (direction == CardinalDirection::north) {
		center.x = x + (float)((roomWidth + 2) % 2) / 2;
		center.y = y - ((float)roomHeight + 2) / 2;
	} else if (direction == CardinalDirection::east) {
		center.x = x + ((float)roomWidth + 2) / 2;
		center.y = y + (float)((roomHeight + 2) % 2) / 2;
	} else if (direction == CardinalDirection::south) {
		center.x = x + (float)((roomWidth + 2) % 2) / 2;
		center.y = y + ((float)roomHeight + 2) / 2;
	} else if (direction == CardinalDirection::west) {
		center.x = x - ((float)roomWidth + 2) / 2;
		center.y = y + (float)((roomHeight + 2) % 2) / 2;
	}
	return center;
}

bool MapGenerator::isInBounds(int x, int y) const
{
	return x >= edgeOffset && x < MAP_SIZE_X - edgeOffset && y >= edgeOffset && y < MAP_SIZE_Y - edgeOffset;
}

/*
 * This function prevents corridors from spawning near the edges of the map that
 * would otherwise create unnecessary dead ends.
 */
bool MapGenerator::isInBounds(int x, int y, int offset, CardinalDirection direction) const
{
	int adjustOffset = offset + edgeOffset;
	if (direction == CardinalDirection::north || direction == CardinalDirection::south) {
		return x >= edgeOffset && x < MAP_SIZE_X - edgeOffset && y >= adjustOffset && y < MAP_SIZE_Y - adjustOffset;
	} else {
		return x >= adjustOffset && x < MAP_SIZE_X - adjustOffset && y >= edgeOffset && y < MAP_SIZE_Y - edgeOffset;
	}
}

void MapGenerator::clearWalls()
{
	for (int i = 0; i < MAP_SIZE_X; i++) {
		for (int j = 0; j < MAP_SIZE_Y; j++) {
			if (getTile(i, j) == 'X') {
				if ((checkNeighbourType(i - 1, j - 1, 'X') | checkNeighbourType(i - 1, j - 1, ' ') | checkNeighbourType(i - 1, j - 1, 'R')) == 255 &&
					(checkNeighbourType(i + 1, j - 1, 'X') | checkNeighbourType(i + 1, j - 1, ' ') | checkNeighbourType(i + 1, j - 1, 'R')) == 255 &&
					(checkNeighbourType(i + 1, j + 1, 'X') | checkNeighbourType(i + 1, j + 1, ' ') | checkNeighbourType(i + 1, j + 1, 'R')) == 255 &&
					(checkNeighbourType(i - 1, j + 1, 'X') | checkNeighbourType(i - 1, j + 1, ' ') | checkNeighbourType(i - 1, j + 1, 'R')) == 255) {
					dungeonTiles_[i + MAP_SIZE_X * j] = ' ';
				}
			}
		}
	}
}

unsigned int MapGenerator::checkNeighbourType(int x, int y, char tileType)
{
	unsigned int neighbours = 0;

	if (getTile(x - 1, y - 1) == tileType) { neighbours += 128; };
	if (getTile(x, y - 1) == tileType    ) { neighbours += 64; };
	if (getTile(x + 1, y - 1) == tileType) { neighbours += 32; };
	if (getTile(x - 1, y) == tileType    ) { neighbours += 16; };
	if (getTile(x + 1, y) == tileType    ) { neighbours += 8; };
	if (getTile(x - 1, y + 1) == tileType) { neighbours += 4; };
	if (getTile(x, y + 1) == tileType    ) { neighbours += 2; };
	if (getTile(x + 1, y + 1) == tileType) { neighbours += 1; };
	return neighbours;
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

bool MapGenerator::adjustPosition(int &x, int &y, int xStart, int yStart, int xEnd, int yEnd, char tileType, CardinalDirection direction)
{
	bool adjustedBackwards = false;
	bool adjustedForwards = false;
	sf::Vector2i point = checkTiles(tileType, xStart, yStart, xEnd, yEnd);
	while (point.x != -1) {
		if (direction == CardinalDirection::north || direction == CardinalDirection::south) {
			if (point.x > x) {
				if (adjustedForwards == true) {
					//std::cout << "Avoided infinite loop" << std::endl;
					return false;
				}
				x--;
				xStart--;
				xEnd--;
				adjustedBackwards = true;
				//std::cout << "moved position west" << std::endl;
			}
			else if (point.x < x)  {
				if (adjustedBackwards == true) {
					//std::cout << "Avoided infinite loop" << std::endl;
					return false;
				}
				x++;
				xStart++;
				xEnd++;
				adjustedForwards = true;
				//std::cout << "moved position east" << std::endl;
			}
			else {
				//std::cout << "could not adjust position" << std::endl;
				return false;
			}
		}
		else if (direction == CardinalDirection::east || direction == CardinalDirection::west) {
			if (point.y > y) {
				if (adjustedForwards == true) {
					//std::cout << "Avoided infinite loop" << std::endl;
					return false;
				}
				y--;
				yStart--;
				yEnd--;
				adjustedBackwards = true;
				//std::cout << "moved position north" << std::endl;
			}
			else if (point.y < y)  {
				if (adjustedBackwards == true) {
					//std::cout << "Avoided infinite loop" << std::endl;
					return false;
				}
				y++;
				yStart++;
				yEnd++;
				adjustedForwards = true;
				//std::cout << "moved position south" << std::endl;
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

sf::Vector2i MapGenerator::checkTiles(char tileType, int xStart, int yStart, int xEnd, int yEnd)
{
	sf::Vector2i coords(-1, -1);
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
				return coords;
			}
		}
	}
	return coords;
}

/*
 * This function will create a room based on the refence coordinates, x and y,
 * as well as the direction. It will first find the starting and ending
 * coordinates to be passed for the setTiles function.
 */
bool MapGenerator::generateRoomCenter(int x, int y)
{
	int roomWidth = random->generate(minRoomWidth_, maxRoomWidth_);
	int roomHeight = random->generate(maxRoomHeight_, maxRoomHeight_);

	int offset;

	if (minRoomHeight_ < minRoomWidth_) {
		offset = minRoomHeight_ + 1;
	}
	else {
		offset = minRoomWidth_ + 1;
	}

	int xOffset = random->generate(-offset, offset);
	int yOffset = random->generate(-offset, offset);

	int xStart = x - roomWidth / 2 + xOffset;
	int yStart = y - roomHeight / 2 + yOffset;
	int xEnd = x + roomWidth / 2 + xOffset;
	int yEnd = y + roomWidth / 2 + yOffset;

	setTiles(xStart, yStart, xEnd, yEnd, '.');
	return true;
}

bool MapGenerator::generateTeleportRoom(int x, int y, int width, int height, int wallWidth, CardinalDirection direction)
{
	if (!spawnCreated) {
		if (generateRoom(x, y, width, height, wallWidth, 'S', 'R', direction)) {
			spawnCreated = true;
			//std::cout << "Spawn created" << std::endl;
			return true;
		}
	} else if (!goalCreated) {
		if (generateRoom(x, y, width, height, wallWidth, 'G', 'R', direction)) {
			goalCreated = true;
			//std::cout << "Goal created" << std::endl;
			return true;
		}
	}
	return false;
}

bool MapGenerator::generateRoom(int x, int y, int width, int height, int wallWidth, char floorType, char wallType, CardinalDirection direction)
{
	int corridorWidth = random->generate(minCorridorWidth_, maxCorridorWidth_);
	int offset = roomEntranceLength_;
	char entranceTile = '.';
	bool adjust = false;
	if (floorType == 'S' || floorType == 'G') {
		offset = 0;
		entranceTile = 'D';
		adjust = true;
	}

	int xStart, yStart, xEnd, yEnd;

	if (direction == CardinalDirection::north) {
		if (adjust == true) {
			adjustPosition(x, y, x - corridorWidth / 2, y + 1, x + (corridorWidth + 1) / 2, y + 1, '.', direction);
		}
		xStart = x - width / 2;
		yStart = y - height - wallWidth - offset;
		xEnd = x + (width + 1) / 2;
		yEnd = y - wallWidth - offset;
	} else if (direction == CardinalDirection::east) {
		if (adjust == true) {
			adjustPosition(x, y, x - 1, y - corridorWidth / 2, x - 1, y + (corridorWidth + 1) / 2, '.', direction);
		}
		xStart = x + wallWidth + offset;
		yStart = y - height / 2;
		xEnd = x + width + wallWidth + offset;
		yEnd = y + (height + 1) / 2;
		} else if (direction == CardinalDirection::south) {
		if (adjust == true) {
			adjustPosition(x, y, x - corridorWidth / 2, y - 1, x + (corridorWidth + 1) / 2, y - 1, '.', direction);
		}
		xStart = x - width / 2;
		yStart = y + wallWidth + offset;
		xEnd = x + (width + 1) / 2;
		yEnd = y + height + wallWidth + offset;
		} else if (direction == CardinalDirection::west) {
		if (adjust == true) {
			adjustPosition(x, y, x + 1, y - corridorWidth / 2, x + 1, y + (corridorWidth + 1) / 2, '.', direction);
		}
		xStart = x - width - wallWidth - offset;
		yStart = y - height / 2;
		xEnd = x - wallWidth - offset;
		yEnd = y + (height + 1) / 2;
	} else {
		std::cout << "Could not set coords for room creation" << std::endl;
	}

	if (!isAreaType(xStart - wallWidth, yStart - wallWidth, xEnd + wallWidth, yEnd + wallWidth, 'X')) {
		//std::cout << "Could not create room" << std::endl;
		return false;
	}

	setTiles(xStart - wallWidth, yStart - wallWidth, xEnd + wallWidth, yEnd + wallWidth, wallType);

	if (!generateCorridor(x, y, corridorWidth, wallWidth + offset - 1, 1, entranceTile, wallType, direction)) {
		return false;
	}

	setTiles(xStart, yStart, xEnd, yEnd, floorType);

	if (floorType == 'S') {
		spawn = getRoomCenter(x, y, width, height, direction);
	}

	//std::cout << "Room created between [" << xStart + wallWidth << "," << yStart + wallWidth << "] and [" << xEnd - wallWidth << "," << yEnd - wallWidth << "]." << std::endl;
	//std::cout << "  Width: " << width << ", height: " << height << ", wall width: " << wallWidth << "." << std::endl;
	return true;
}

bool MapGenerator::generateCorridor(int x, int y, int width, int length, int wallWidth, char floorType, char wallType, CardinalDirection direction)
{
	int offset = wallWidth;

	int xStart = x;
	int yStart = y;
	int xEnd = x;
	int yEnd = y;

	int xOffset = 0;
	int yOffset = 0;

	if (direction == CardinalDirection::north) {
		adjustPosition(x, y, x - width / 2, y + 1, x + (width + 1) / 2, y + 1, '.', direction);
		xStart = x - width / 2;
		yStart = y - length;
		xEnd = x + (width + 1) / 2;
		xOffset = offset;
	} else if (direction == CardinalDirection::east) {
		adjustPosition(x, y, x - 1, y - width / 2, x - 1, y + (width + 1) / 2, '.', direction);;
		yStart = y - width / 2;
		xEnd = x + length;
		yEnd = y + (width + 1) / 2;
		yOffset = offset;
	} else if (direction == CardinalDirection::south) {
		adjustPosition(x, y, x - width / 2, y - 1, x + (width + 1) / 2, y - 1, '.', direction);
		xStart = x - width / 2;
		xEnd = x + (width + 1) / 2;
		yEnd = y + length;
		xOffset = offset;
	} else if (direction == CardinalDirection::west) {
		adjustPosition(x, y, x + 1, y - width / 2, x + 1, y + (width + 1) / 2, '.', direction);
		xStart = x - length;
		yStart = y - width / 2;
		yEnd = y + (width + 1) / 2;
		yOffset = offset;
	}

	if (minRoomHeight_ < minRoomWidth_) {
		offset = minRoomHeight_ + 1;
	}
	else {
		offset = minRoomWidth_ + 1;
	}

	if (!isInBounds(xStart, yStart, offset, direction) || !isInBounds(xEnd, yEnd, offset, direction)) {
		//std::cout << "Could not create corridor." << std::endl;
		return false;
	}

	if (!isAreaType(xStart - xOffset, yStart - yOffset, xEnd + xOffset, yEnd + yOffset, wallType)) {
		//std::cout << "Could not create corridor." << std::endl;
		return false;
	}

	setTiles(xStart, yStart, xEnd, yEnd, floorType);
	//std::cout << "Corridor created between [" << xStart << "," << yStart << "] and [" << xEnd << "," << yEnd << "]." << std::endl;
	//std::cout << "  Width: " << width << ", Length: " << length << "." << std::endl;
	return true;
}

/*
 * Due to a bug, all features facing north or west are drawn 1 tile to far from
 * the target, this has been patched with a workaround in setTiles function
 *
 * This function will first pick a random wall tile that is adjacent to a room
 * or corridor. Then it will create a feature facing either north, east, south
 * or west, depending on which side of the room or corridor that wall is.
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
			if (generateFeature(x, y, CardinalDirection::north)) {
				return true;
			}
		}
		else if (getTile(x - 1, y) == '.') {
			//std::cout << "Generating feature facing east at [" << x << "," << y << "]" << std::endl;
			if (generateFeature(x, y, CardinalDirection::east)) {
				return true;
			}
		}
		else if (getTile(x, y - 1) == '.') {
			//std::cout << "Generating feature facing south at [" << x << "," << y << "]" << std::endl;
			if (generateFeature(x, y, CardinalDirection::south)) {
				return true;
			}
		}
		else if (getTile(x + 1, y) == '.') {
			//std::cout << "Generating feature facing west at [" << x << "," << y << "]" << std::endl;
			if (generateFeature(x, y, CardinalDirection::west)) {
				return true;
			}
		}
	}
	return false;
}


/*
 * This function will roll a number and create either a room or corridor based
 * on the outcome. The chance for the feature can be adjusted in the constructor
 */
bool MapGenerator::generateFeature(int x, int y, CardinalDirection direction)
{
	int maxRoll = 100;
	int minRoll = 1;
	int roll = random->generate(minRoll, maxRoll);

	if (roll >= corridorChance_ && roll <= corridorChance_ + roomChance_){
		int roomWall = random->generate(minRoomWall_, maxRoomWall_);
		if (generateRoom(x, y, random->generate(minRoomWidth_, maxRoomWidth_), random->generate(maxRoomHeight_, maxRoomHeight_), roomWall, '.', 'X', direction)) {
			return true;
		}
		return false;
	} else if (roll <= corridorChance_) {
		int corridorWall = random->generate(minCorridorWall_, maxCorridorWall_);
		if (generateCorridor(x, y, random->generate(minCorridorWidth_, maxCorridorWidth_), random->generate(minCorridorLength_, maxCorridorLength_), corridorWall, '.', 'X', direction)) {
			return true;
		}
		return false;
	} else if (roll >= maxRoll - teleportRoomChance_) {
		int roomSide = 3;
		if (!generateTeleportRoom(x, y, roomSide, roomSide, 1, direction)) {
			return true;
		}
		return false;
	} else {
		std::cout << "Corridor, room and teleportroom chance totals less than 100, please adjust the values" << std::endl;
	}
}

char* MapGenerator::generateMap()
{
	while (!goalCreated) {
		spawnCreated = false;
		goalCreated = false;
		fillTiles(' ');
		setTiles(edgeOffset, edgeOffset, MAP_SIZE_X - edgeOffset, MAP_SIZE_Y - edgeOffset, 'X');
		generateRoomCenter(MAP_SIZE_X / 2, MAP_SIZE_Y / 2);

		for (int features = 1; features != maxFeatures_; features++) {
			if (!generateFeature()) {
				break;
			}
		}

		clearWalls();

		if (!goalCreated) {
			delete random;
			random = new Random();
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
