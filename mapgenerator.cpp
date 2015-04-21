#include "mapgenerator.hpp"
#include <iostream>
#include <fstream>

MapGenerator::MapGenerator() {
	maxFeatures_ = 10000;
	maxTries_ = 5000;
	roomChance_ = 75;
	corridorChance_ = 25;

	random = new Random();

	dungeonTiles_ = new char*[MAP_SIZE_X + 1];
	for (int i = 0; i < MAP_SIZE_X + 1; i++) {
		dungeonTiles_[i] = new char[MAP_SIZE_Y];
	}
}

MapGenerator::~MapGenerator() {
	for (int i = 0; i < MAP_SIZE_X + 1; i++) {
		delete [] dungeonTiles_[i];
	}
	delete random;
	delete [] dungeonTiles_;
}

int MapGenerator::setTile(int x, int y, char tileType) {
	dungeonTiles_[x][y] = tileType;
	return 0;
}

int MapGenerator::setTiles(int xStart, int yStart, int xEnd, int yEnd, char tileType) {
	for (int i = xStart; i != xEnd + 1; ++i) {
		for (int j = yStart; j != yEnd + 1; ++j) {
			dungeonTiles_[i][j] = tileType;
		}
	}
	return 0;
}

int MapGenerator::fillTiles(char tileType) {
	for (int i = 0; i < MAP_SIZE_X; i++) {
		for (int j = 0; j < MAP_SIZE_Y; j++) {
			dungeonTiles_[i][j] = tileType;
		}
	}
	return 0;
}

char MapGenerator::getTile(int x, int y) {
	return dungeonTiles_[x][y];
}

Direction MapGenerator::getRandDirection() {
	return Direction(random->generate(0, 3));
}

bool MapGenerator::isInBounds(int x, int y) const {
	return x >= 1 && x < MAP_SIZE_X - 1 && y >= 1 && y < MAP_SIZE_Y - 1;
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

unsigned int MapGenerator::checkNeighbourType(int x, int y, char tileType) {
	unsigned int neighbours = 0;

	if (dungeonTiles_[x - 1][y - 1] == tileType) { neighbours += 128; };
	if (dungeonTiles_[x]	[y - 1] == tileType) { neighbours += 64; };
	if (dungeonTiles_[x + 1][y - 1] == tileType) { neighbours += 32; };
	if (dungeonTiles_[x - 1]	[y] == tileType) { neighbours += 16; };
	if (dungeonTiles_[x + 1]	[y] == tileType) { neighbours += 8; };
	if (dungeonTiles_[x - 1][y + 1] == tileType) { neighbours += 4; };
	if (dungeonTiles_[x]	[y + 1] == tileType) { neighbours += 2; };
	if (dungeonTiles_[x + 1][y + 1] == tileType) { neighbours += 1; };
	return neighbours;
}

bool MapGenerator::generateRoom(int x, int y, int xMaxLength, int yMaxLength, Direction direction) {
	int xLength = random->generate(6, xMaxLength);
	int yLength = random->generate(6, yMaxLength);

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
		return false;
	}

	setTiles(xStart, yStart, xEnd, yEnd, 'X');
	setTiles(xStart + 1, yStart + 1, xEnd - 1, yEnd - 1, '.');
	return true;
}

bool MapGenerator::generateCorridor(int x, int y, int maxLength, Direction direction) {
	int length = random->generate(4, maxLength);
	int width = 2;
	int offset = 2;

	int xStart = x;
	int yStart = y;

	int xEnd = x;
	int yEnd = y;

	int xStartOffset = 0;
	int yStartOffset = 0;
	int xEndOffset = 0;
	int yEndOffset = 0;

	if (direction == Direction::north) {
		xEnd = x + width;
		yStart = y - length;
		xStartOffset = -offset;
		xEndOffset = offset;
	}
	else if (direction == Direction::east) {
		xEnd = x + length;
		yEnd = y + width;
		yStartOffset = -offset;
		yEndOffset = offset;
	}
	else if (direction == Direction::south) {
		xEnd = x + width;
		yEnd = y + length;
		xStartOffset = -offset;
		xEndOffset = offset;
	}
	else if (direction == Direction::west) {
		xStart = x - length;
		yEnd = y + width;
		yStartOffset = -offset;
		yEndOffset = offset;
	}

	if (!isAreaType(xStart - xStartOffset, yStart - yStartOffset, xEnd + xEndOffset, yEnd + yEndOffset, 'X')) {
		return false;
	}

	setTiles(xStart, yStart, xEnd, yEnd, '.');
	return true;
}

bool MapGenerator::generateFeature() {
	int x = 0;
	int y = 0;

	/*Pick a wall adjacent to a room*/
	for (int tries = 0; tries != maxTries_; ++tries) {
		x = random->generate(1, MAP_SIZE_X - 1);
		y = random->generate(1, MAP_SIZE_Y - 1);

		if (dungeonTiles_[x][y] != 'X') {
			continue;
		}

		if (isAdjacent(x, y, '.')) {
		}

		if (dungeonTiles_[x][y + 1] == '.') {
			if (generateFeature(x, y, 0, -1, Direction::north)) {
				return true;
			}
		}
		else if (dungeonTiles_[x - 1][y] == '.') {
			if (generateFeature(x, y, 1, 0, Direction::east)) {
				return true;
			}
		}
		else if (dungeonTiles_[x][y - 1] == '.') {
			if (generateFeature(x, y, 0, 1, Direction::south)) {
				return true;
			}
		}
		else if (dungeonTiles_[x + 1][y] == '.') {
			if (generateFeature(x, y, -1, 0, Direction::west)) {
				return true;
			}
		}
	}
	return false;
}

bool MapGenerator::generateFeature(int x, int y, int xOffset, int yOffset, Direction direction) {
	int roll = random->generate(0, 100);

	if (roll <= CORRIDOR_CHANCE) {
			if (generateCorridor(x, y, 5, direction)) {
				return true;
			}
		return false;
	}
	else {
		if (generateRoom(x + xOffset, y + yOffset, 10, 10, direction)) {
			dungeonTiles_[x][y] = '.';
				dungeonTiles_[x + xOffset][y + yOffset] = '.';
			return true;
		}
		return false;
	}
}


char** MapGenerator::generateMap() {
	fillTiles('X');
	setTile(MAP_SIZE_X / 2, MAP_SIZE_Y / 2, '.');
	generateRoom(MAP_SIZE_X / 2, MAP_SIZE_Y / 2, random->generate(6,10), random->generate(6,10), getRandDirection());
	generateFeature();
	for (int features = 1; features != maxFeatures_; features++) {
		if (!generateFeature()) {
			break;
		}
	}
	return dungeonTiles_;
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
