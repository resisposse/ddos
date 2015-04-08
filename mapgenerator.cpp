#include "mapgenerator.hpp"
#include <iostream>
#include <fstream>
#include <random>

MapGenerator::MapGenerator() {
	seed_ = std::random_device() ();
	maxFeatures_ = 10000;
	roomChance_ = 75;
	corridorChance_ = 25;

	dungeonTiles_ = new char*[MAP_SIZE_X + 1];
	for (int i = 0; i < MAP_SIZE_X + 1; i++) {
		dungeonTiles_[i] = new char[MAP_SIZE_Y];
	}
}

MapGenerator::~MapGenerator() {
	for (int i = 0; i < MAP_SIZE_X + 1; i++) {
		delete [] dungeonTiles_[i];
	}
	delete [] dungeonTiles_;
}

bool MapGenerator::isInBounds(int x, int y) const {
	return x >= 1 && x < MAP_SIZE_X - 1 && y >= 1 && y < MAP_SIZE_Y - 1;
}

/*x and y must not be more than MAX_SIZE_X or MAX_SIZE_Y*/
int MapGenerator::setTile(int x, int y, char tileType) {
	dungeonTiles_[x][y] = tileType;
	return 0;
}

int MapGenerator::setTiles(int xStart, int yStart, int xEnd, int yEnd, char tileType) {
	/*
	if (xStart >= xEnd) {
		std::cout << "Error: xStart is greater than xEnd" << std::endl;
		return 1;
	}
	else if (yStart >= yEnd) {
		std::cout << "Error: yStart is greater than yEnd" << std::endl;
		return 1;
	}
	*/
	std::cout << "Hi" << std::endl;

	for (int i = xStart; i < xEnd; i++) {
		for (int j = yStart; j < yEnd; j++) {
			dungeonTiles_[i][j] = tileType;
			//std::cout << "Set tile to \'" << tileType << "\' at coords \[" << i << "," << j << "\]." << std::endl;
		}
	}
	return 0;
}

/*x and y must not be more than MAX_SIZE_X or MAX_SIZE_Y*/
char MapGenerator::getTile(int x, int y) {
	return dungeonTiles_[x][y];
}

int MapGenerator::getRand(int min, int max) {
	std::random_device rd;
	std::mt19937 rng(rd());
	return std::uniform_int_distribution<int>(min, max) (rng);

	//time_t seed;
	//seed = time(NULL) + oldseed;
	//oldseed = seed;
	//srand(seed);

	//int result = min + rand() % (max - min + 1);
	//return result;
}

Direction MapGenerator::getRandDirection() {
	return Direction(getRand(0, 3));
}

int MapGenerator::fillTiles(char tileType) {
	for (int i = 0; i < MAP_SIZE_X; i++) {
		for (int j = 0; j < MAP_SIZE_Y; j++) {
			dungeonTiles_[i][j] = tileType;
		}
	}
	return 0;
}

bool MapGenerator::eliminateDeadEnds() {
	int x;
	int y;
	int maxTries = 5000;
	for (int tries = 0; tries != maxTries; ++tries) {
		x = getRand(1, MAP_SIZE_X - 1);
		y = getRand(1, MAP_SIZE_Y - 1);

		if (isAdjacent(x, y, '.')) {
			std::cout << "found at " << x << "," << y << std::endl;
		}

		if (x < MAP_SIZE_X / 2 && y < MAP_SIZE_Y) {

		}
	}
	return false;
}

bool MapGenerator::generateFeature() {
	int x = 0;
	int y = 0;
	int maxTries = 5000;

	/*Pick a wall adjacent to a room*/
	for (int tries = 0; tries != maxTries; ++tries) {
		x = getRand(1, MAP_SIZE_X - 1);
		y = getRand(1, MAP_SIZE_Y - 1);
		
		//std::cout << x << "," << y << std::endl;

		if (dungeonTiles_[x][y] != 'X') {
			//std::cout << "not found at " << x << "," << y << std::endl;
			continue;
		}

		if (isAdjacent(x, y, '.')) {
			std::cout << "found at " << x << "," << y << std::endl;
		}

		if (dungeonTiles_[x][y + 1] == '.') {
			std::cout << "generating feature facing north" << std::endl;
			if (generateFeature(x, y, 0, 0, Direction::north)) {
				return true;
			}
		}
		else if (dungeonTiles_[x - 1][y] == '.') {
			std::cout << "generating feature facing east" << std::endl;
			if (generateFeature(x, y, 1, 0, Direction::east)) {
				return true;
			}
		}
		else if (dungeonTiles_[x][y - 1] == '.') {
			std::cout << "generating feature facing south" << std::endl;
			if (generateFeature(x, y, 0, 1, Direction::south)) {
				return true;
			}
		}
		else if (dungeonTiles_[x + 1][y] == '.') {
			std::cout << "generating feature facing west" << std::endl;
			if (generateFeature(x, y, 0, 0, Direction::west)) {
				return true;
			}
		}
	}
	return false;
}

bool MapGenerator::generateFeature(int x, int y, int xOffset, int yOffset, Direction direction) {
	int roll = getRand(0, 100);

	if (roll <= CORRIDOR_CHANCE) {
		std::cout << "Corridor rolled" << std::endl;
		if (generateCorridor(x + xOffset, y + yOffset, 5, direction)) {
			dungeonTiles_[x][y] = '.';
			return true;
		}
		return false;
	}
	else {
		std::cout << "Room rolled" << std::endl;
		if (generateRoom(x + xOffset, y + yOffset, 10, 10, direction)) {
			dungeonTiles_[x][y] = '.';
			if (direction == Direction::north) {
				dungeonTiles_[x][y - 1] = '.';
			}
			else if (direction == Direction::west) {
				dungeonTiles_[x - 1][y] = '.';
			}
			else {
				dungeonTiles_[x + xOffset][y + yOffset] = '.';
			}
			return true;
		}
		return false;
	}
}

bool MapGenerator::generateRoom(int x, int y, int xMaxLength, int yMaxLength, Direction direction) {
	int xLength = getRand(4, xMaxLength);
	int yLength = getRand(4, yMaxLength);

	std::cout << "Room size: " << xLength << " * " << yLength << std::endl;

	int xStart = x;
	int yStart = y;

	int xEnd = x;
	int yEnd = y;

	if (direction == Direction::north) {
		xStart = x - xLength / 2;
		yStart = y - yLength;
		xEnd = x + (xLength + 1) / 2;
	}
	else if (direction == Direction::east) {
		yStart = y - yLength / 2;
		xEnd = x + xLength;
		yEnd = y + (yLength + 1) / 2;
	}
	else if (direction == Direction::south) {
		xStart = x - xLength / 2;
		xEnd = x + (xLength + 1) / 2;
		yEnd = y + yLength;
	}
	else if (direction == Direction::west) {
		xStart = x - xLength;
		yStart = y - yLength / 2;
		yEnd = y + (yLength + 1) / 2;
	}
	else {
		std::cout << "Could not set coords for room creation" << std::endl;
	}

	if (!isAreaType(xStart, yStart, xEnd, yEnd, 'X')) {
		std::cout << "Room failed" << std::endl;
		return false;
	}

	setTiles(xStart, yStart, xEnd, yEnd, 'X');
	setTiles(xStart + 1, yStart + 1, xEnd - 1, yEnd - 1, '.');
	std::cout << "Room created" << std::endl;
	return true;
}

bool MapGenerator::generateCorridor(int x, int y, int maxLength, Direction direction) {
	int length = getRand(4, maxLength);
	std::cout << "Corridor length: " << length << std::endl;

	int xStart = x;
	int yStart = y;

	int xEnd = x;
	int yEnd = y;

	if (direction == Direction::north) {
		xEnd = x + 1;
		yStart = y - length;
	}
	else if (direction == Direction::east) {
		xEnd = x + length;
		yEnd = y + 1;
	}
	else if (direction == Direction::south) {
		xEnd = x + 1;
		yEnd = y + length;
	}
	else if (direction == Direction::west) {
		xStart = x - length;
		yEnd = y + 1;
	}

	if (!isAreaType(xStart - 1, yStart - 1, xEnd + 1, yEnd + 1, 'X')) {
		std::cout << "Corridor failed" << std::endl;
		return false;
	}

	setTiles(xStart, yStart, xEnd, yEnd, '.');
	std::cout << "Corridor created" << std::endl;
	return true;
}

char** MapGenerator::generateMap() {
	fillTiles('X');
	generateRoom(MAP_SIZE_X / 2, MAP_SIZE_Y / 2, getRand(4,8), getRand(4,8), getRandDirection());
	generateFeature();
	for (int features = 1; features != maxFeatures_; features++) {
		if (!generateFeature()) {
			std::cout << "Can't place anymore features. Placed " << features << " features" << std::endl;
			break;
		}
	}
	return dungeonTiles_;
}

bool MapGenerator::isAreaType(int xStart, int yStart, int xEnd, int yEnd, char tileType) {
	if (!isInBounds(xStart, yStart)) {
		return false;
	}
	else if (!isInBounds(xEnd, yEnd)) {
		return false;
	}

	for (int i = xStart; i < xEnd; i++) {
		for (int j = yStart; j < yEnd; j++) {
			if (dungeonTiles_[i][j] != tileType) {
				std::cout << "[" << i << "," << j << "] is occupied" << std::endl;
				return false;
			}
		}
	}
	return true;
}

bool MapGenerator::isAdjacent(int x, int y, char tileType){
	int i = x;
	int j = y;
	if (dungeonTiles_[i][j - 1] == tileType ||
		dungeonTiles_[i - 1][j] == tileType ||
		dungeonTiles_[i + 1][j] == tileType ||
		dungeonTiles_[i][j + 1] == tileType)
		return true;
	else
		return false;
}

int MapGenerator::writeMapFile(char (&dungeonTiles_)[MAP_SIZE_Y + 1][MAP_SIZE_Y + 1]) {
	std::ofstream out;
	out.open("map.dat");
	for (int i = 0; i < MAP_SIZE_X; i++) {
		for (int j = 0; j < MAP_SIZE_Y + 1; j++) {
			out.put(dungeonTiles_[i][j]);
		}
	}
	out.close();
	return 0;
}
