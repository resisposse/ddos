/*
 * Project Name
 * 2015 © Project Team (see: LICENSE)
 */

#include <SFML/Graphics.hpp>
#include "fog.hpp"
#include "map.hpp"
#include "object.hpp"
#include "main.hpp"
#include "math.h"
#include <iostream>

Map::Map()
{
	srand((unsigned)time(NULL));

	tileMapTex = new sf::Texture();
	tileMapTex->loadFromFile("media/ddos-tiles1.png");

	bgTex = new sf::Texture();
	bgTex->loadFromFile("media/ddos-bg.png");
	bgSpr = new sf::Sprite(*bgTex);

	sf::IntRect floor           (TILE_SIZE * 5, TILE_SIZE * 5, TILE_SIZE, TILE_SIZE);
	sf::IntRect floorMetal      (TILE_SIZE * 5, TILE_SIZE * 2, TILE_SIZE, TILE_SIZE);
	sf::IntRect floorDamagedV1	(TILE_SIZE * 10, TILE_SIZE * 6, TILE_SIZE, TILE_SIZE);
	sf::IntRect floorDamagedV2	(TILE_SIZE * 11, TILE_SIZE * 6, TILE_SIZE, TILE_SIZE);
	sf::IntRect floorDamagedV3	(TILE_SIZE * 12, TILE_SIZE * 6, TILE_SIZE, TILE_SIZE);
	sf::IntRect floorDamagedV4	(TILE_SIZE * 13, TILE_SIZE * 6, TILE_SIZE, TILE_SIZE);
	sf::IntRect floorDamagedV5	(TILE_SIZE * 14, TILE_SIZE * 6, TILE_SIZE, TILE_SIZE);
	
	sf::IntRect wallMiddle      (TILE_SIZE * 2, TILE_SIZE * 1, TILE_SIZE, TILE_SIZE);
	
	sf::IntRect wallHorizontal1 (TILE_SIZE * 4, TILE_SIZE * 0, TILE_SIZE, TILE_SIZE);
	sf::IntRect wallHorizontal2 (TILE_SIZE * 5, TILE_SIZE * 0, TILE_SIZE, TILE_SIZE);
	sf::IntRect wallHorizontal3 (TILE_SIZE * 6, TILE_SIZE * 0, TILE_SIZE, TILE_SIZE);
	sf::IntRect wallHorizontal4 (TILE_SIZE * 7, TILE_SIZE * 0, TILE_SIZE, TILE_SIZE);
	sf::IntRect wallHorizontal5 (TILE_SIZE * 8, TILE_SIZE * 0, TILE_SIZE, TILE_SIZE);

	sf::IntRect wallVertical1	(TILE_SIZE * 3, TILE_SIZE * 2, TILE_SIZE, TILE_SIZE);
	sf::IntRect wallVertical2	(TILE_SIZE * 3, TILE_SIZE * 3, TILE_SIZE, TILE_SIZE);
	sf::IntRect wallVertical3	(TILE_SIZE * 3, TILE_SIZE * 4, TILE_SIZE, TILE_SIZE);
	sf::IntRect wallVertical4	(TILE_SIZE * 3, TILE_SIZE * 5, TILE_SIZE, TILE_SIZE);
	sf::IntRect wallVertical5	(TILE_SIZE * 3, TILE_SIZE * 6, TILE_SIZE, TILE_SIZE);

	sf::IntRect wallCorner1		(TILE_SIZE * 2, TILE_SIZE * 0, TILE_SIZE, TILE_SIZE);
	sf::IntRect wallCorner2		(TILE_SIZE * 3, TILE_SIZE * 0, TILE_SIZE, TILE_SIZE);
	sf::IntRect wallCorner3		(TILE_SIZE * 2, TILE_SIZE * 1, TILE_SIZE, TILE_SIZE);
	sf::IntRect wallCorner4		(TILE_SIZE * 3, TILE_SIZE * 1, TILE_SIZE, TILE_SIZE);

	sf::IntRect wallFill1		(TILE_SIZE * 2, TILE_SIZE * 2, TILE_SIZE, TILE_SIZE);
	sf::IntRect wallFill2		(TILE_SIZE * 2, TILE_SIZE * 3, TILE_SIZE, TILE_SIZE);
	sf::IntRect wallFill3		(TILE_SIZE * 2, TILE_SIZE * 4, TILE_SIZE, TILE_SIZE);
	sf::IntRect wallFill4		(TILE_SIZE * 2, TILE_SIZE * 5, TILE_SIZE, TILE_SIZE);
	sf::IntRect wallFill5		(TILE_SIZE * 2, TILE_SIZE * 6, TILE_SIZE, TILE_SIZE);

	floorSpr           = new sf::Sprite(*tileMapTex, floor);
	floorMetalSpr      = new sf::Sprite(*tileMapTex, floorMetal);
	floorDamagedV1Spr  = new sf::Sprite(*tileMapTex, floorDamagedV1);
	floorDamagedV2Spr  = new sf::Sprite(*tileMapTex, floorDamagedV2);
	floorDamagedV3Spr  = new sf::Sprite(*tileMapTex, floorDamagedV3);
	floorDamagedV4Spr  = new sf::Sprite(*tileMapTex, floorDamagedV4);
	floorDamagedV5Spr  = new sf::Sprite(*tileMapTex, floorDamagedV5);
	floorTiles.push_back(floorSpr);
	floorTiles.push_back(floorDamagedV1Spr);
	floorTiles.push_back(floorDamagedV2Spr);
	floorTiles.push_back(floorDamagedV3Spr);
	floorTiles.push_back(floorDamagedV4Spr);
	floorTiles.push_back(floorDamagedV5Spr);

	wallMiddleSpr      = new sf::Sprite(*tileMapTex, wallMiddle);

	wallHorizontal1Spr = new sf::Sprite(*tileMapTex, wallHorizontal1);
	wallHorizontal2Spr = new sf::Sprite(*tileMapTex, wallHorizontal2);
	wallHorizontal3Spr = new sf::Sprite(*tileMapTex, wallHorizontal3);
	wallHorizontal4Spr = new sf::Sprite(*tileMapTex, wallHorizontal4);
	wallHorizontal5Spr = new sf::Sprite(*tileMapTex, wallHorizontal5);
	wallHorizontalTiles.push_back(wallHorizontal1Spr);
	wallHorizontalTiles.push_back(wallHorizontal2Spr);
	wallHorizontalTiles.push_back(wallHorizontal3Spr);
	wallHorizontalTiles.push_back(wallHorizontal4Spr);
	wallHorizontalTiles.push_back(wallHorizontal5Spr);

	wallVertical1Spr = new sf::Sprite(*tileMapTex, wallVertical1);
	wallVertical2Spr = new sf::Sprite(*tileMapTex, wallVertical2);
	wallVertical3Spr = new sf::Sprite(*tileMapTex, wallVertical3);
	wallVertical4Spr = new sf::Sprite(*tileMapTex, wallVertical4);
	wallVertical5Spr = new sf::Sprite(*tileMapTex, wallVertical5);
	wallVerticalTiles.push_back(wallVertical1Spr);
	wallVerticalTiles.push_back(wallVertical2Spr);
	wallVerticalTiles.push_back(wallVertical3Spr);
	wallVerticalTiles.push_back(wallVertical4Spr);
	wallVerticalTiles.push_back(wallVertical5Spr);

	wallCorner1Spr = new sf::Sprite(*tileMapTex, wallCorner1);
	wallCorner2Spr = new sf::Sprite(*tileMapTex, wallCorner2);
	wallCorner3Spr = new sf::Sprite(*tileMapTex, wallCorner3);
	wallCorner4Spr = new sf::Sprite(*tileMapTex, wallCorner4);
	wallCornerTiles.push_back(wallCorner1Spr);
	wallCornerTiles.push_back(wallCorner2Spr);
	wallCornerTiles.push_back(wallCorner3Spr);
	wallCornerTiles.push_back(wallCorner4Spr);

	wallFill1Spr = new sf::Sprite(*tileMapTex, wallFill1);
	wallFill2Spr = new sf::Sprite(*tileMapTex, wallFill2);
	wallFill3Spr = new sf::Sprite(*tileMapTex, wallFill3);
	wallFill4Spr = new sf::Sprite(*tileMapTex, wallFill4);
	wallFill5Spr = new sf::Sprite(*tileMapTex, wallFill5);
	wallFillTiles.push_back(wallFill1Spr);
	wallFillTiles.push_back(wallFill2Spr);
	wallFillTiles.push_back(wallFill3Spr);
	wallFillTiles.push_back(wallFill4Spr);
	wallFillTiles.push_back(wallFill5Spr);

	char _tiles[MAP_SIZE_X][MAP_SIZE_Y + 1] = {
		"                                  XXXX",
		"      XXXXXXXXXXXXXXXXXXXXXXXXXX XXXXX",
		"      X........XXXXXXXXXXXXXXXXXXXXXXX",
		"      X...00000...X..............XXXXX",
		"      X...........X..............XXXXX",
		"      X.......XX..XXXXXXXXXXXXX..XXXXX",
		"XXXXXXX..XXXXXXX..XXXXXXXXXX.......XXX",
		"XXXXXXX..XXXXXXX..XXXXXXXXXX.......XXX",
		"X........XXXXXXX..X   XXXXXX.......XXX",
		"X..XXXX..XXXXX X..X    XXXXX.......XXX",
		"X..X  X..XXX   X..X     XXXX.......XXX",
		"X..X  X..XXXX  X..X   XXXXXX.......XXX",
		"X..XXXX...XXXXXX..XXXXXXXXXX.......XXX",
		"X.....0...XXXXXX..XXXXXXXXXX.......XXX",
		"X...00000.........X................XXX",
		"X.....0...........X................XXX",
		"X.........XXXXXXXXXXXXXXXXXX.......XXX",
		"X.........XXX   XXX....XXXXXXX.....XXX",
		"X.........XXXXXXXXX....XXXXXXX.....XXX",
		"X....XXXXXXX....XXX....XXXXXXX.....XXX",
		"X....X..........XXX....XXXXXXX.....XXX",
		"X...0X0.........XXX....XXXXXXX.....XXX",
		"X...00...XXXX...XXX....XXXXXXX.....XXX",
		"X........XXXXXXXXXX................XXX",
		"XXXXXXXXXXXXXXXXXXX................XXX",
		"X...000...XXXXXXXXX................XXX",
		"X...00....XXXXXXXXX....XXXXXXX.....XXX",
		"X...000................XXXXXXX.....XXX",
		"X...00.................XXXXXXX.....XXX",
		"X......................XXXXXXX0000.XXX",
		"X.........XXXXXXXXXXXXXXXXXXXX.....XXX",
		"X.........XXXXXXXXXXXXXXXXXXXX.....XXX",
		"XXXXXXXXXXXXXXXXXXXXXXXXXXX........XXX",
		"XXXXXXXXXXXXXXXXXXXXXXXXXXX........XXX",
		"XX........XXXXX.....XXXXXXX........XXX",
		"XX........XXXXX....................XXX",
		"XX........XXXXX....................XXX",
		"XX........XXXXX.............000....XXX",
		"XX..00....XXXXX.....XXXXXXX........XXX",
		"XX........XXXXX.....XXXXXXX........XXX",
		"XX........XXXXX.....XXXXXXX........XXX",
		"XX..00....XXXXX.....XXXXXXX........XXX",
		"XX........XXXXXXXXXXXXXXXXX....00..XXX",
		"XX........XXXXXXXXXXXXXXXXX........XXX",
		"XX..0000.......0000................XXX",
		"XX.............0000................XXX",
		"XX.................................XXX",
		"XX........XXXXXXXXXXXXXXXXX........XXX",
		"XX........XXXXXXXXXXXXXXXXX........XXX",
		"XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
	};
	for (int i = 0; i < MAP_SIZE_X; i++) for (int j = 0; j < MAP_SIZE_Y; j++) {
		floorTilesRand[i][j]          = rand() % floorTiles.size();
		wallHorizontalTilesRand[i][j] = rand() % wallHorizontalTiles.size();
		wallVerticalTilesRand[i][j]	  = rand() % wallVerticalTiles.size();
		wallCornerTilesRand[i][j]     = rand() % wallCornerTiles.size();
		wallFillTilesRand[i][j]	      = rand() % wallFillTiles.size();
		tiles[i][j].index = sf::Vector2i(i, j);
		switch (_tiles[i][j]) {
		case 'X':
			tiles[i][j].type = mtWall;
			tiles[i][j].absorb = 35;
			collisionMap[i][j] = 1;
			break;
		case '.':
			tiles[i][j].type = mtFloor;
			tiles[i][j].absorb = 6;
			collisionMap[i][j] = 0;
			break;
		case ' ':
			tiles[i][j].type = mtAir;
			tiles[i][j].absorb = 35;
			collisionMap[i][j] = 0;
			break;
		case '0':
			tiles[i][j].type = mtFloorMetal;
			tiles[i][j].absorb = 6;
			collisionMap[i][j] = 0;
		}
	}
	int lightCount = MAP_SIZE_X * MAP_SIZE_Y;
	for (int i = 0; i < LIGHT_MAX_LIGHTLEVEL; i++) {
		lightTiles[i] = new MapTile*[lightCount];
	}
}

Map::~Map()
{
	for (int i = 0; i < LIGHT_MAX_LIGHTLEVEL; delete lightTiles[i++]);
	delete bgTex;
	delete bgSpr;
	delete floorSpr;
	delete floorMetalSpr;
	delete floorDamagedV1Spr;
	delete floorDamagedV2Spr;
	delete floorDamagedV3Spr;
	delete floorDamagedV4Spr;
	delete floorDamagedV5Spr;
	delete wallMiddleSpr;
	delete wallHorizontal1Spr;
	delete wallHorizontal2Spr;
	delete wallHorizontal3Spr;
	delete wallHorizontal4Spr;
	delete wallHorizontal5Spr;
	delete wallVertical1Spr;
	delete wallVertical2Spr;
	delete wallVertical3Spr;
	delete wallVertical4Spr;
	delete wallVertical5Spr;
	delete wallCorner1Spr;
	delete wallCorner2Spr;
	delete wallCorner3Spr;
	delete wallCorner4Spr;
	delete wallFill1Spr;
	delete wallFill2Spr;
	delete wallFill3Spr;
	delete wallFill4Spr;
	delete wallFill5Spr;
	deleteList(sources);
}

/* TODO Add more types of wall types, like T-shaped etc. */
int Map::updateWallDirection(MapTile tile)
{
	/*
	 * Tile positions
	 * [1 2 3]
	 * [4 5 6]
	 * [7 8 9]
	 */
	int a = 5;
	int x = tile.index.x;
	int y = tile.index.y;

	if (tiles[x - 1][y].type != tile.type && tiles[x][y - 1].type != tile.type &&
	    tiles[x + 1][y].type == tile.type && tiles[x][y + 1].type == tile.type) {
		a = 1;
	} else if (tiles[x - 1][y].type == tile.type && tiles[x][y - 1].type != tile.type &&
	           tiles[x + 1][y].type == tile.type) {
		a = 2;
	} else if (tiles[x - 1][y].type == tile.type && tiles[x][y - 1].type != tile.type &&
	           tiles[x + 1][y].type != tile.type && tiles[x][y + 1].type == tile.type) {
		a = 3;
	} else if (tiles[x - 1][y].type != tile.type && tiles[x][y - 1].type == tile.type &&
	                                                tiles[x][y + 1].type == tile.type) {
		a = 4;
	} else if (tiles[x - 1][y].type == tile.type && tiles[x][y - 1].type == tile.type &&
	           tiles[x + 1][y].type != tile.type && tiles[x][y + 1].type == tile.type) {
		a = 6;
	} else if (tiles[x - 1][y].type != tile.type && tiles[x][y - 1].type == tile.type &&
	           tiles[x + 1][y].type == tile.type && tiles[x][y + 1].type != tile.type) {
		a = 7;
	} else if (tiles[x - 1][y].type == tile.type &&
	           tiles[x + 1][y].type == tile.type && tiles[x][y + 1].type != tile.type) {
		a = 8;
	} else if (tiles[x - 1][y].type == tile.type && tiles[x][y - 1].type == tile.type &&
	           tiles[x + 1][y].type != tile.type && tiles[x][y + 1].type != tile.type) {
		a = 9;
	} else {
		a = 5;
	}
	return a;
}

void Map::update(StaticLightSource *tmpSource)
{
	updateList(sources);
	resetLight();
	render();
	checkSources(tmpSource);
	light();
}

void Map::render()
{
	renderTiles();
}

void Map::checkSources(StaticLightSource *tmpSource)
{
	for (unsigned int i = 0; i < sources.size(); i++) {
		addIntensity(sources[i]->position, sources[i]->getIntensity(), sources[i]->color);
	}
	if (sf::IntRect(0, 0, MAP_SIZE_X, MAP_SIZE_Y).contains(tmpSource->position)) {
		addIntensity(tmpSource->position, tmpSource->intensity, tmpSource->color);
	}
}

void Map::renderTiles()
{
	sf::Color tileColor;
	app->draw(*bgSpr);
	for (int i = 0; i < MAP_SIZE_X; i++) for (int j = 0; j < MAP_SIZE_Y; j++) {
		tileColor = sf::Color::White;
		switch (tiles[i][j].type) {
		case mtWall:
			//tileColor = sf::Color(140, 140, 140, 255);
			/* TODO Condense this switch case to a function. */
			switch (direction = updateWallDirection(tiles[i][j])){
			case 1:
				//drawTile(wallCornerTiles, wallCornerTilesRand, i, j, tileColor);
				drawWallCornerTile(i, j, tileColor);
				break;
			case 2:
				//drawTile(wallHorizontalTiles, wallHorizontalTilesRand, i, j, tileColor);
				drawWallHorizontalTile(i, j, tileColor); 
				break;
			case 3:
				//drawTile(wallCornerTiles, wallCornerTilesRand, i, j, tileColor);
				drawWallCornerTile(i, j, tileColor);
				break;
			case 4:
				//drawTile(wallVerticalTiles, wallVerticalTilesRand, i, j, tileColor);
				drawWallVerticalTile(i, j, tileColor);
				break;
			case 5:
				if (checkNeighbourType(tiles[i][j], mtWall) == 11111111) {
					//drawTile(wallFillTiles, wallFillTilesRand, i, j, tileColor);
					drawWallFillTile(i, j, tileColor);
				} else {
					wallMiddleSpr->setPosition(i * TILE_SIZE, j * TILE_SIZE);
					wallMiddleSpr->setColor(tileColor);
					app->draw(*wallMiddleSpr);
				}
				break;
			case 6:
				//drawTile(wallVerticalTiles, wallVerticalTilesRand, i, j, tileColor);
				drawWallVerticalTile(i, j, tileColor);
				break;
			case 7:
				//drawTile(wallCornerTiles, wallCornerTilesRand, i, j, tileColor);
				drawWallCornerTile(i, j, tileColor); 
				break;
			case 8:
				//drawTile(wallHorizontalTiles, wallHorizontalTilesRand, i, j, tileColor);
				drawWallHorizontalTile(i, j, tileColor); 
				break;
			case 9:
				//drawTile(wallCornerTiles, wallCornerTilesRand, i, j, tileColor);
				drawWallCornerTile(i, j, tileColor); 
				break;
			}
			break;
		case mtFloor:
			if (checkNeighbourType(tiles[i][j], mtWall) > 0) {
				floorMetalSpr->setPosition(i * TILE_SIZE, j * TILE_SIZE);
				floorMetalSpr->setColor(tileColor);
				app->draw(*floorMetalSpr);
			} else {
				//drawTile(floorTiles, floorTilesRand, i, j, tileColor);
				drawFloorTile(i, j, tileColor);
			}
			break;
		case mtFloorMetal:
			floorMetalSpr->setPosition(i * TILE_SIZE, j * TILE_SIZE);
			floorMetalSpr->setColor(tileColor);
			app->draw(*floorMetalSpr);
			break;
		case mtAir:
			break;
		}
	}
}

int Map::Collision(float X, float Y) {
	float x = X;
	float y = Y;
	int collision = 0;

	//top
	int i = x - 9;
	int j = y - 10;

	if (collision == 0) {
		for (i; i < x + 9; i++) {
			if (collisionMap[i / TILE_SIZE][j / TILE_SIZE] == 1){
				collision = 1;
				break;
			}
		}
	}
	//left
	if (collision == 0) {
		i = x - 10;
		j = y - 10;
		for (j; j < y + 10; j++) {
			if (collisionMap[i / TILE_SIZE][j / TILE_SIZE] == 1){
				collision = 1;
				break;
			}
		}
	}
	//right
	if (collision == 0) {
		i = x + 10;
		j = y - 10;
		for (j; j < y + 10; j++) {
			if (collisionMap[i / TILE_SIZE][j / TILE_SIZE] == 1){
				collision = 1;
				break;
			}
		}
	}

	//bottom
	if (collision == 0) {
		j = y + 10;
		i = x - 9;
		for (i; i < x + 9; i++) {
			if (collisionMap[i / TILE_SIZE][j / TILE_SIZE] == 1){
				collision = 1;
				break;
			}
		}
	}

	return collision;
}

int Map::checkNeighbourType(MapTile tile, MapTileType tileType){
	int neighbours = 0;
	int x = tile.index.x;
	int y = tile.index.y;

	if (tiles[x - 1][y - 1].type == tileType) { neighbours += 10000000; };
	if (tiles[x]	[y - 1].type == tileType) { neighbours += 1000000; };
	if (tiles[x + 1][y - 1].type == tileType) { neighbours += 100000; };
	if (tiles[x - 1][y].type == tileType) {		neighbours += 10000; };
	if (tiles[x + 1][y].type == tileType) {		neighbours += 1000; };
	if (tiles[x - 1][y + 1].type == tileType) { neighbours += 100; };
	if (tiles[x]	[y + 1].type == tileType) { neighbours += 10; };
	if (tiles[x + 1][y + 1].type == tileType) { neighbours += 1; };
	return neighbours;
}

void Map::drawFloorTile(int i, int j, sf::Color tileColor) {
	int type = floorTilesRand[i][j];
	floorTiles[type]->setPosition(i * TILE_SIZE, j * TILE_SIZE);
	floorTiles[type]->setColor(tileColor);
	app->draw(*floorTiles[type]);
}

void Map::drawWallCornerTile(int i, int j, sf::Color tileColor) {
	int type = wallCornerTilesRand[i][j];
	wallCornerTiles[type]->setPosition(i * TILE_SIZE, j * TILE_SIZE);
	wallCornerTiles[type]->setColor(tileColor);
	app->draw(*wallCornerTiles[type]);
}

void Map::drawWallHorizontalTile(int i, int j, sf::Color tileColor) {
	int type = wallHorizontalTilesRand[i][j];
	wallHorizontalTiles[type]->setPosition(i * TILE_SIZE, j * TILE_SIZE);
	wallHorizontalTiles[type]->setColor(tileColor);
	app->draw(*wallHorizontalTiles[type]);
}

void Map::drawWallVerticalTile(int i, int j, sf::Color tileColor) {
	int type = wallVerticalTilesRand[i][j];
	wallVerticalTiles[type]->setPosition(i * TILE_SIZE, j * TILE_SIZE);
	wallVerticalTiles[type]->setColor(tileColor);
	app->draw(*wallVerticalTiles[type]);
}

void Map::drawWallFillTile(int i, int j, sf::Color tileColor){
	int type = wallFillTilesRand[i][j];
	wallFillTiles[type]->setPosition(i * TILE_SIZE, j * TILE_SIZE);
	wallFillTiles[type]->setColor(tileColor);
	app->draw(*wallFillTiles[type]);
}

/* When in debug mode causes significant slowdowns.
 * Running the program in release mode seems to work 
 * fine. Vectors in debug mode have extra checks for
 * safety. */
void Map::drawTile(std::vector<sf::Sprite *> tileVector, int tileVectorRand[MAP_SIZE_X][MAP_SIZE_Y], int i, int j, sf::Color tileColor) {
	int type = tileVectorRand[i][j];
	tileVector[type]->setPosition(i * TILE_SIZE, j * TILE_SIZE);
	tileVector[type]->setColor(tileColor);
	app->draw(*tileVector[type]);
}