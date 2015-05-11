/*
 * Dark Domains Of Space
 * 2015 � Project Team (see: LICENSE)
 */

#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
#include <cmath>
#include "map.hpp"
#include "main.hpp"

Map::Map(char *generatedMap)
{
	srand((unsigned)time(NULL));

	tileMapTex = new sf::Texture();
	tileMapTex->loadFromFile("media/ddos-tiles1.png");

	lavaTex = new sf::Texture();
	lavaTex->loadFromFile("media/ddos-lava.png");

	bgTex = new sf::Texture();
	bgTex->loadFromFile("media/ddos-bg.png");
	bgSpr = new sf::Sprite(*bgTex);

	sf::IntRect floor      (TILE_SIZE * 5, TILE_SIZE * 5, TILE_SIZE, TILE_SIZE);
	sf::IntRect floorMetal (TILE_SIZE * 5, TILE_SIZE * 2, TILE_SIZE, TILE_SIZE);

	sf::IntRect floorGrateVer         (TILE_SIZE * 15, TILE_SIZE * 4, TILE_SIZE, TILE_SIZE);
	sf::IntRect floorGrateHor         (TILE_SIZE * 16, TILE_SIZE * 4, TILE_SIZE, TILE_SIZE);
	sf::IntRect floorGrateTop         (TILE_SIZE * 15, TILE_SIZE * 5, TILE_SIZE, TILE_SIZE);
	sf::IntRect floorGrateTopEmpty    (TILE_SIZE * 15, TILE_SIZE * 6, TILE_SIZE, TILE_SIZE);
	sf::IntRect floorGrateBottomEmpty (TILE_SIZE * 16, TILE_SIZE * 6, TILE_SIZE, TILE_SIZE);
	sf::IntRect floorGrateLeft        (TILE_SIZE * 16, TILE_SIZE * 5, TILE_SIZE, TILE_SIZE);
	sf::IntRect floorGrateTopRight    (TILE_SIZE * 18, TILE_SIZE * 6, TILE_SIZE, TILE_SIZE);

	sf::IntRect floorDamagedV1 (TILE_SIZE * 10, TILE_SIZE * 6, TILE_SIZE, TILE_SIZE);
	sf::IntRect floorDamagedV2 (TILE_SIZE * 11, TILE_SIZE * 6, TILE_SIZE, TILE_SIZE);
	sf::IntRect floorDamagedV3 (TILE_SIZE * 12, TILE_SIZE * 6, TILE_SIZE, TILE_SIZE);
	sf::IntRect floorDamagedV4 (TILE_SIZE * 13, TILE_SIZE * 6, TILE_SIZE, TILE_SIZE);
	sf::IntRect floorDamagedV5 (TILE_SIZE * 14, TILE_SIZE * 6, TILE_SIZE, TILE_SIZE);

	sf::IntRect floorVar1  (TILE_SIZE * 15, TILE_SIZE * 0, TILE_SIZE, TILE_SIZE);
	sf::IntRect floorVar2  (TILE_SIZE * 16, TILE_SIZE * 0, TILE_SIZE, TILE_SIZE);
	sf::IntRect floorVar3  (TILE_SIZE * 17, TILE_SIZE * 0, TILE_SIZE, TILE_SIZE);
	sf::IntRect floorVar4  (TILE_SIZE * 18, TILE_SIZE * 0, TILE_SIZE, TILE_SIZE);
	sf::IntRect floorVar5  (TILE_SIZE * 15, TILE_SIZE * 1, TILE_SIZE, TILE_SIZE);
	sf::IntRect floorVar6  (TILE_SIZE * 16, TILE_SIZE * 1, TILE_SIZE, TILE_SIZE);
	sf::IntRect floorVar7  (TILE_SIZE * 17, TILE_SIZE * 1, TILE_SIZE, TILE_SIZE);
	sf::IntRect floorVar8  (TILE_SIZE * 18, TILE_SIZE * 1, TILE_SIZE, TILE_SIZE);
	sf::IntRect floorVar9  (TILE_SIZE * 15, TILE_SIZE * 2, TILE_SIZE, TILE_SIZE);
	sf::IntRect floorVar10 (TILE_SIZE * 16, TILE_SIZE * 2, TILE_SIZE, TILE_SIZE);
	sf::IntRect floorVar11 (TILE_SIZE * 17, TILE_SIZE * 2, TILE_SIZE, TILE_SIZE);
	sf::IntRect floorVar12 (TILE_SIZE * 18, TILE_SIZE * 2, TILE_SIZE, TILE_SIZE);
	sf::IntRect floorVar13 (TILE_SIZE * 15, TILE_SIZE * 3, TILE_SIZE, TILE_SIZE);
	sf::IntRect floorVar14 (TILE_SIZE * 16, TILE_SIZE * 3, TILE_SIZE, TILE_SIZE);

	sf::IntRect floorTopLeft     (TILE_SIZE * 4,  TILE_SIZE * 1, TILE_SIZE, TILE_SIZE);
	sf::IntRect floorTop         (TILE_SIZE * 5,  TILE_SIZE * 1, TILE_SIZE, TILE_SIZE);
	sf::IntRect floorTopRight    (TILE_SIZE * 6,  TILE_SIZE * 1, TILE_SIZE, TILE_SIZE);
	sf::IntRect floorLeft        (TILE_SIZE * 4,  TILE_SIZE * 2, TILE_SIZE, TILE_SIZE);
	sf::IntRect floorRight       (TILE_SIZE * 6,  TILE_SIZE * 2, TILE_SIZE, TILE_SIZE);
	sf::IntRect floorBottomLeft  (TILE_SIZE * 4,  TILE_SIZE * 3, TILE_SIZE, TILE_SIZE);
	sf::IntRect floorBottom      (TILE_SIZE * 5,  TILE_SIZE * 3, TILE_SIZE, TILE_SIZE);
	sf::IntRect floorBottomRight (TILE_SIZE * 6,  TILE_SIZE * 3, TILE_SIZE, TILE_SIZE);

	sf::IntRect floorTopLeftCorner     (TILE_SIZE * 7, TILE_SIZE * 1, TILE_SIZE, TILE_SIZE);
	sf::IntRect floorTopRightCorner    (TILE_SIZE * 8, TILE_SIZE * 1, TILE_SIZE, TILE_SIZE);
	sf::IntRect floorBottomLeftCorner  (TILE_SIZE * 7, TILE_SIZE * 2, TILE_SIZE, TILE_SIZE);
	sf::IntRect floorBottomRightCorner (TILE_SIZE * 8, TILE_SIZE * 2, TILE_SIZE, TILE_SIZE);

	sf::IntRect wallMiddle (TILE_SIZE * 2, TILE_SIZE * 1, TILE_SIZE, TILE_SIZE);

	sf::IntRect wallHorizontal1 (TILE_SIZE * 4, TILE_SIZE * 0, TILE_SIZE, TILE_SIZE);
	sf::IntRect wallHorizontal2 (TILE_SIZE * 5, TILE_SIZE * 0, TILE_SIZE, TILE_SIZE);
	sf::IntRect wallHorizontal3 (TILE_SIZE * 6, TILE_SIZE * 0, TILE_SIZE, TILE_SIZE);
	sf::IntRect wallHorizontal4 (TILE_SIZE * 7, TILE_SIZE * 0, TILE_SIZE, TILE_SIZE);
	sf::IntRect wallHorizontal5 (TILE_SIZE * 8, TILE_SIZE * 0, TILE_SIZE, TILE_SIZE);

	sf::IntRect wallVertical1 (TILE_SIZE * 3, TILE_SIZE * 2, TILE_SIZE, TILE_SIZE);
	sf::IntRect wallVertical2 (TILE_SIZE * 3, TILE_SIZE * 3, TILE_SIZE, TILE_SIZE);
	sf::IntRect wallVertical3 (TILE_SIZE * 3, TILE_SIZE * 4, TILE_SIZE, TILE_SIZE);
	sf::IntRect wallVertical4 (TILE_SIZE * 3, TILE_SIZE * 5, TILE_SIZE, TILE_SIZE);
	sf::IntRect wallVertical5 (TILE_SIZE * 3, TILE_SIZE * 6, TILE_SIZE, TILE_SIZE);

	sf::IntRect wallCorner1	(TILE_SIZE * 2, TILE_SIZE * 0, TILE_SIZE, TILE_SIZE);
	sf::IntRect wallCorner2 (TILE_SIZE * 3, TILE_SIZE * 0, TILE_SIZE, TILE_SIZE);
	sf::IntRect wallCorner3 (TILE_SIZE * 2, TILE_SIZE * 1, TILE_SIZE, TILE_SIZE);
	sf::IntRect wallCorner4 (TILE_SIZE * 3, TILE_SIZE * 1, TILE_SIZE, TILE_SIZE);

	sf::IntRect wallFill1 (TILE_SIZE * 2, TILE_SIZE * 2, TILE_SIZE, TILE_SIZE);
	sf::IntRect wallFill2 (TILE_SIZE * 2, TILE_SIZE * 3, TILE_SIZE, TILE_SIZE);
	sf::IntRect wallFill3 (TILE_SIZE * 2, TILE_SIZE * 4, TILE_SIZE, TILE_SIZE);
	sf::IntRect wallFill4 (TILE_SIZE * 2, TILE_SIZE * 5, TILE_SIZE, TILE_SIZE);
	sf::IntRect wallFill5 (TILE_SIZE * 2, TILE_SIZE * 6, TILE_SIZE, TILE_SIZE);

	sf::IntRect lavaFrame1 (TILE_SIZE * 0, TILE_SIZE * 0, TILE_SIZE, TILE_SIZE);
	sf::IntRect lavaFrame2 (TILE_SIZE * 1, TILE_SIZE * 0, TILE_SIZE, TILE_SIZE);
	sf::IntRect lavaFrame3 (TILE_SIZE * 2, TILE_SIZE * 0, TILE_SIZE, TILE_SIZE);
	sf::IntRect lavaFrame4 (TILE_SIZE * 3, TILE_SIZE * 0, TILE_SIZE, TILE_SIZE);
	sf::IntRect lavaFrame5 (TILE_SIZE * 4, TILE_SIZE * 0, TILE_SIZE, TILE_SIZE);
	sf::IntRect lavaFrame6 (TILE_SIZE * 5, TILE_SIZE * 0, TILE_SIZE, TILE_SIZE);
	sf::IntRect lavaFrame7 (TILE_SIZE * 6, TILE_SIZE * 0, TILE_SIZE, TILE_SIZE);
	sf::IntRect lavaFrame8 (TILE_SIZE * 7, TILE_SIZE * 0, TILE_SIZE, TILE_SIZE);

	floorSpr = new sf::Sprite(*tileMapTex, floor);
	floorSprCpy = new sf::Sprite(*tileMapTex, floor);
	floorMetalSpr = new sf::Sprite(*tileMapTex, floorMetal);
	floorGrateHorSpr = new sf::Sprite(*tileMapTex, floorGrateHor);
	floorGrateVerSpr = new sf::Sprite(*tileMapTex, floorGrateVer);
	floorGrateTopSpr = new sf::Sprite(*tileMapTex, floorGrateTop);
	floorGrateTopEmptySpr = new sf::Sprite(*tileMapTex, floorGrateTopEmpty);
	floorGrateBottomEmptySpr = new sf::Sprite(*tileMapTex, floorGrateBottomEmpty);
	floorGrateLeftSpr = new sf::Sprite(*tileMapTex, floorGrateLeft);
	floorGrateTopRightSpr = new sf::Sprite(*tileMapTex, floorGrateTopRight);
	floorDamagedV1Spr = new sf::Sprite(*tileMapTex, floorDamagedV1);
	floorDamagedV2Spr = new sf::Sprite(*tileMapTex, floorDamagedV2);
	floorDamagedV3Spr = new sf::Sprite(*tileMapTex, floorDamagedV3);
	floorDamagedV4Spr = new sf::Sprite(*tileMapTex, floorDamagedV4);
	floorDamagedV5Spr = new sf::Sprite(*tileMapTex, floorDamagedV5);
	floorVar1Spr = new sf::Sprite(*tileMapTex, floorVar1);
	floorVar2Spr = new sf::Sprite(*tileMapTex, floorVar2);
	floorVar3Spr = new sf::Sprite(*tileMapTex, floorVar3);
	floorVar4Spr = new sf::Sprite(*tileMapTex, floorVar4);
	floorVar5Spr = new sf::Sprite(*tileMapTex, floorVar5);
	floorVar6Spr = new sf::Sprite(*tileMapTex, floorVar6);
	floorVar7Spr = new sf::Sprite(*tileMapTex, floorVar7);
	floorVar8Spr = new sf::Sprite(*tileMapTex, floorVar8);
	floorVar9Spr = new sf::Sprite(*tileMapTex, floorVar9);
	floorVar10Spr = new sf::Sprite(*tileMapTex, floorVar10);
	floorVar11Spr = new sf::Sprite(*tileMapTex, floorVar11);
	floorVar12Spr = new sf::Sprite(*tileMapTex, floorVar12);
	floorVar13Spr = new sf::Sprite(*tileMapTex, floorVar13);
	floorVar14Spr = new sf::Sprite(*tileMapTex, floorVar14);
	floorTopLeftSpr = new sf::Sprite(*tileMapTex, floorTopLeft);
	floorTopSpr = new sf::Sprite(*tileMapTex, floorTop);
	floorTopRightSpr = new sf::Sprite(*tileMapTex, floorTopRight);
	floorLeftSpr = new sf::Sprite(*tileMapTex, floorLeft);
	floorRightSpr = new sf::Sprite(*tileMapTex, floorRight);
	floorBottomLeftSpr = new sf::Sprite(*tileMapTex, floorBottomLeft);
	floorBottomSpr = new sf::Sprite(*tileMapTex, floorBottom);
	floorBottomRightSpr = new sf::Sprite(*tileMapTex, floorBottomRight);
	floorTopLeftCornerSpr = new sf::Sprite(*tileMapTex, floorTopLeftCorner);
	floorTopRightCornerSpr = new sf::Sprite(*tileMapTex, floorTopRightCorner);
	floorBottomLeftCornerSpr = new sf::Sprite(*tileMapTex, floorBottomLeftCorner);
	floorBottomRightCornerSpr = new sf::Sprite(*tileMapTex, floorBottomRightCorner);
	floorTiles.push_back(floorMetalSpr);
	floorTiles.push_back(floorMetalSpr);
	floorTiles.push_back(floorMetalSpr);
	floorTiles.push_back(floorMetalSpr);
	floorTiles.push_back(floorMetalSpr);
	floorTiles.push_back(floorMetalSpr);
	floorTiles.push_back(floorMetalSpr);
	floorTiles.push_back(floorMetalSpr);
	floorTiles.push_back(floorMetalSpr);
	floorTiles.push_back(floorMetalSpr);
	floorTiles.push_back(floorMetalSpr);
	floorTiles.push_back(floorMetalSpr);
	floorTiles.push_back(floorMetalSpr);
	floorTiles.push_back(floorMetalSpr);
	floorTiles.push_back(floorMetalSpr);
	floorTiles.push_back(floorMetalSpr);
	floorTiles.push_back(floorMetalSpr);
	floorTiles.push_back(floorMetalSpr);
	floorTiles.push_back(floorMetalSpr);
	floorTiles.push_back(floorMetalSpr);
	floorTiles.push_back(floorSpr);
	floorTiles.push_back(floorSpr);
	floorTiles.push_back(floorSpr);
	floorTiles.push_back(floorSpr);
	floorTiles.push_back(floorSpr);
	floorTiles.push_back(floorSpr);
	floorTiles.push_back(floorSpr);
	floorTiles.push_back(floorSprCpy);
	floorTiles.push_back(floorDamagedV1Spr);
	floorTiles.push_back(floorDamagedV2Spr);
	floorTiles.push_back(floorDamagedV3Spr);
	floorTiles.push_back(floorDamagedV4Spr);
	floorTiles.push_back(floorDamagedV5Spr);
	floorTiles.push_back(floorVar1Spr);
	floorTiles.push_back(floorVar2Spr);
	floorTiles.push_back(floorVar3Spr);
	floorTiles.push_back(floorVar4Spr);
	floorTiles.push_back(floorVar5Spr);
	floorTiles.push_back(floorVar6Spr);
	floorTiles.push_back(floorVar7Spr);
	floorTiles.push_back(floorVar8Spr);
	floorTiles.push_back(floorVar9Spr);
	floorTiles.push_back(floorVar10Spr);
	floorTiles.push_back(floorVar11Spr);
	floorTiles.push_back(floorVar12Spr);
	floorTiles.push_back(floorVar13Spr);
	floorTiles.push_back(floorVar14Spr);

	wallMiddleSpr = new sf::Sprite(*tileMapTex, wallMiddle);

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

	lava1Spr = new sf::Sprite(*lavaTex, lavaFrame1);
	lava2Spr = new sf::Sprite(*lavaTex, lavaFrame2);
	lava3Spr = new sf::Sprite(*lavaTex, lavaFrame3);
	lava4Spr = new sf::Sprite(*lavaTex, lavaFrame4);
	lava5Spr = new sf::Sprite(*lavaTex, lavaFrame5);
	lava6Spr = new sf::Sprite(*lavaTex, lavaFrame6);
	lava7Spr = new sf::Sprite(*lavaTex, lavaFrame7);
	lava8Spr = new sf::Sprite(*lavaTex, lavaFrame8);
	lavaFrames.push_back(lava1Spr);
	lavaFrames.push_back(lava2Spr);
	lavaFrames.push_back(lava3Spr);
	lavaFrames.push_back(lava4Spr);
	lavaFrames.push_back(lava5Spr);
	lavaFrames.push_back(lava6Spr);
	lavaFrames.push_back(lava7Spr);
	lavaFrames.push_back(lava8Spr);

	for (int i = 0; i < MAP_SIZE_X; i++) for (int j = 0; j < MAP_SIZE_Y; j++) {
		floorTilesRand[i][j]          = rand() % floorTiles.size();
		wallHorizontalTilesRand[i][j] = rand() % wallHorizontalTiles.size();
		wallVerticalTilesRand[i][j]   = rand() % wallVerticalTiles.size();
		wallCornerTilesRand[i][j]     = rand() % wallCornerTiles.size();
		wallFillTilesRand[i][j]	      = rand() % wallFillTiles.size();

		//floorTilesRand[i][j]          = randBellCurve(floorTiles.size());
		tiles[i][j].index = sf::Vector2i(i, j);
		switch (generatedMap[i + MAP_SIZE_X * j]) {
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
			break;
		case 'L':
			tiles[i][j].type = mtLava;
			tiles[i][j].absorb = 6;
			collisionMap[i][j] = 0;
		}
	}
}

Map::~Map()
{
	delete bgTex;
	delete bgSpr;
	delete floorSpr;
	delete floorSprCpy;
	delete floorMetalSpr;
	delete floorGrateHorSpr;
	delete floorGrateVerSpr;
	delete floorGrateTopSpr;
	delete floorGrateTopEmptySpr;
	delete floorGrateBottomEmptySpr;
	delete floorGrateLeftSpr;
	delete floorGrateTopRightSpr;
	delete floorDamagedV1Spr;
	delete floorDamagedV2Spr;
	delete floorDamagedV3Spr;
	delete floorDamagedV4Spr;
	delete floorDamagedV5Spr;
	delete floorVar1Spr;
	delete floorVar2Spr;
	delete floorVar3Spr;
	delete floorVar4Spr;
	delete floorVar5Spr;
	delete floorVar6Spr;
	delete floorVar7Spr;
	delete floorVar8Spr;
	delete floorVar9Spr;
	delete floorVar10Spr;
	delete floorVar11Spr;
	delete floorVar12Spr;
	delete floorVar13Spr;
	delete floorVar14Spr;
	delete floorTopLeftSpr;
	delete floorTopSpr;
	delete floorTopRightSpr;
	delete floorLeftSpr;
	delete floorRightSpr;
	delete floorBottomLeftSpr;
	delete floorBottomSpr;
	delete floorBottomRightSpr;
	delete floorTopLeftCornerSpr;
	delete floorTopRightCornerSpr;
	delete floorBottomLeftCornerSpr;
	delete floorBottomRightCornerSpr;
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
	delete lava1Spr;
	delete lava2Spr;
	delete lava3Spr;
	delete lava4Spr;
	delete lava5Spr;
	delete lava6Spr;
	delete lava7Spr;
	delete lava8Spr;
}

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
				if (checkNeighbourType(tiles[i][j], mtWall) == 255) {
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
			if ((checkNeighbourType(tiles[i][j], mtWall) & 80) == 80) {
				floorTopLeftSpr->setPosition(i * TILE_SIZE, j * TILE_SIZE);
				floorTopLeftSpr->setColor(tileColor);
				app->draw(*floorTopLeftSpr);
			} else if ((checkNeighbourType(tiles[i][j], mtWall) & 72) == 72) {
				floorTopRightSpr->setPosition(i * TILE_SIZE, j * TILE_SIZE);
				floorTopRightSpr->setColor(tileColor);
				app->draw(*floorTopRightSpr);
			} else if ((checkNeighbourType(tiles[i][j], mtWall) & 66) == 66) {
				floorGrateHorSpr->setPosition(i * TILE_SIZE, j * TILE_SIZE);
				floorGrateHorSpr->setColor(tileColor);
				app->draw(*floorGrateHorSpr);
			} else if ((checkNeighbourType(tiles[i][j], mtWall) & 97) == 97) {
				floorGrateTopRightSpr->setPosition(i * TILE_SIZE, j * TILE_SIZE);
				floorGrateTopRightSpr->setColor(tileColor);
				app->draw(*floorGrateTopRightSpr);
			} else if ((checkNeighbourType(tiles[i][j], mtWall) & 64) == 64) {
				floorTopSpr->setPosition(i * TILE_SIZE, j * TILE_SIZE);
				floorTopSpr->setColor(tileColor);
				app->draw(*floorTopSpr);
			} else if ((checkNeighbourType(tiles[i][j], mtWall) & 34) == 34) {
				floorBottomRightSpr->setPosition(i * TILE_SIZE, j * TILE_SIZE);
				floorBottomRightSpr->setColor(tileColor);
				app->draw(*floorBottomRightSpr);
			} else if ((checkNeighbourType(tiles[i][j], mtWall) & 24) == 24) {
				floorGrateVerSpr->setPosition(i * TILE_SIZE, j * TILE_SIZE);
				floorGrateVerSpr->setColor(tileColor);
				app->draw(*floorGrateVerSpr);
			} else if ((checkNeighbourType(tiles[i][j], mtWall) & 18) == 18) {
				floorBottomLeftSpr->setPosition(i * TILE_SIZE, j * TILE_SIZE);
				floorBottomLeftSpr->setColor(tileColor);
				app->draw(*floorBottomLeftSpr);
			} else if ((checkNeighbourType(tiles[i][j], mtWall) & 48) == 48) {
				floorGrateTopEmptySpr->setPosition(i * TILE_SIZE, j * TILE_SIZE);
				floorGrateTopEmptySpr->setColor(tileColor);
				app->draw(*floorGrateTopEmptySpr);
			} else if ((checkNeighbourType(tiles[i][j], mtWall) & 34) == 34) {
				floorBottomRightCornerSpr->setPosition(i * TILE_SIZE, j * TILE_SIZE);
				floorBottomRightCornerSpr->setColor(tileColor);
				app->draw(*floorBottomRightCornerSpr);
			} else if ((checkNeighbourType(tiles[i][j], mtWall) & 17) == 17) {
				floorGrateBottomEmptySpr->setPosition(i * TILE_SIZE, j * TILE_SIZE);
				floorGrateBottomEmptySpr->setColor(tileColor);
				app->draw(*floorGrateBottomEmptySpr);
			} else if ((checkNeighbourType(tiles[i][j], mtWall) & 16) == 16) {
				floorLeftSpr->setPosition(i * TILE_SIZE, j * TILE_SIZE);
				floorLeftSpr->setColor(tileColor);
				app->draw(*floorLeftSpr);
			} else if ((checkNeighbourType(tiles[i][j], mtWall) & 13) == 13) {
				floorBottomRightSpr->setPosition(i * TILE_SIZE, j * TILE_SIZE);
				floorBottomRightSpr->setColor(tileColor);
				app->draw(*floorBottomRightSpr);
			} else if ((checkNeighbourType(tiles[i][j], mtWall) & 12) == 12) {
				floorBottomRightSpr->setPosition(i * TILE_SIZE, j * TILE_SIZE);
				floorBottomRightSpr->setColor(tileColor);
				app->draw(*floorBottomRightSpr);
			} else if ((checkNeighbourType(tiles[i][j], mtWall) & 10) == 10) {
				floorBottomRightSpr->setPosition(i * TILE_SIZE, j * TILE_SIZE);
				floorBottomRightSpr->setColor(tileColor);
				app->draw(*floorBottomRightSpr);
			} else if ((checkNeighbourType(tiles[i][j], mtWall) & 8) == 8) {
				floorRightSpr->setPosition(i * TILE_SIZE, j * TILE_SIZE);
				floorRightSpr->setColor(tileColor);
				app->draw(*floorRightSpr);
			} else if ((checkNeighbourType(tiles[i][j], mtWall) & 2) == 2) {
				floorBottomSpr->setPosition(i * TILE_SIZE, j * TILE_SIZE);
				floorBottomSpr->setColor(tileColor);
				app->draw(*floorBottomSpr);
			} else if ((checkNeighbourType(tiles[i][j], mtWall) & 160) == 160) {
				floorGrateTopSpr->setPosition(i * TILE_SIZE, j * TILE_SIZE);
				floorGrateTopSpr->setColor(tileColor);
				app->draw(*floorGrateTopSpr);
			} else if ((checkNeighbourType(tiles[i][j], mtWall) & 132) == 132) {
				floorGrateLeftSpr->setPosition(i * TILE_SIZE, j * TILE_SIZE);
				floorGrateLeftSpr->setColor(tileColor);
				app->draw(*floorGrateLeftSpr);
			} else if ((checkNeighbourType(tiles[i][j], mtWall) & 128) == 128) {
				floorTopLeftCornerSpr->setPosition(i * TILE_SIZE, j * TILE_SIZE);
				floorTopLeftCornerSpr->setColor(tileColor);
				app->draw(*floorTopLeftCornerSpr);
			} else if ((checkNeighbourType(tiles[i][j], mtWall) & 33) == 33) {
				floorRightSpr->setPosition(i * TILE_SIZE, j * TILE_SIZE);
				floorRightSpr->setColor(tileColor);
				app->draw(*floorRightSpr);
			} else if ((checkNeighbourType(tiles[i][j], mtWall) & 32) == 32) {
				floorTopRightCornerSpr->setPosition(i * TILE_SIZE, j * TILE_SIZE);
				floorTopRightCornerSpr->setColor(tileColor);
				app->draw(*floorTopRightCornerSpr);
			} else if ((checkNeighbourType(tiles[i][j], mtWall) & 5) == 5) {
				floorBottomSpr->setPosition(i * TILE_SIZE, j * TILE_SIZE);
				floorBottomSpr->setColor(tileColor);
				app->draw(*floorBottomSpr);
			} else if ((checkNeighbourType(tiles[i][j], mtWall) & 4) == 4) {
				floorBottomLeftCornerSpr->setPosition(i * TILE_SIZE, j * TILE_SIZE);
				floorBottomLeftCornerSpr->setColor(tileColor);
				app->draw(*floorBottomLeftCornerSpr);
			} else if ((checkNeighbourType(tiles[i][j], mtWall) & 1) == 1) {
				floorBottomRightCornerSpr->setPosition(i * TILE_SIZE, j * TILE_SIZE);
				floorBottomRightCornerSpr->setColor(tileColor);
				app->draw(*floorBottomRightCornerSpr);
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
		case mtLava:
			drawLavaFrame(i, j, tileColor, 0.1);
			break;
		case mtAir:
			break;
		}
	}
}

int Map::collision(float x, float y, std::string collisionType)
{
	int i, j;
	int collision = 0;
	int collisionMargin = 0;

	if (collisionType == "projectile") {
		collisionMargin = 2;
	} else {
		collisionMargin = 10;
	}

	/* Top */
	if (collision == 0) {
		i = x - collisionMargin;
		j = y - collisionMargin;
		for (; i < x + collisionMargin; i++) {
			if (collisionMap[i / TILE_SIZE][j / TILE_SIZE] == 1){
				collision = 1;
				break;
			}
		}
	}

	/* Left */
	if (collision == 0) {
		i = x - collisionMargin;
		j = y - collisionMargin;
		for (; j < y + collisionMargin; j++) {
			if (collisionMap[i / TILE_SIZE][j / TILE_SIZE] == 1){
				collision = 1;
				break;
			}
		}
	}

	/* Right */
	if (collision == 0) {
		i = x + collisionMargin;
		j = y - collisionMargin;
		for (; j < y + collisionMargin; j++) {
			if (collisionMap[i / TILE_SIZE][j / TILE_SIZE] == 1){
				collision = 1;
				break;
			}
		}
	}

	/* Bottom */
	if (collision == 0) {
		j = y + collisionMargin;
		i = x - collisionMargin;
		for (; i < x + collisionMargin; i++) {
			if (collisionMap[i / TILE_SIZE][j / TILE_SIZE] == 1){
				collision = 1;
				break;
			}
		}
	}

	return collision;
}

int Map::checkNeighbourType(MapTile tile, MapTileType tileType)
{
	unsigned int neighbours = 0;
	int x = tile.index.x;
	int y = tile.index.y;

	if (tiles[x - 1][y - 1].type == tileType) { neighbours += 128; };
	if (tiles[x]	[y - 1].type == tileType) { neighbours += 64; };
	if (tiles[x + 1][y - 1].type == tileType) { neighbours += 32; };
	if (tiles[x - 1][y].type == tileType) {		neighbours += 16; };
	if (tiles[x + 1][y].type == tileType) {		neighbours += 8; };
	if (tiles[x - 1][y + 1].type == tileType) { neighbours += 4; };
	if (tiles[x]	[y + 1].type == tileType) { neighbours += 2; };
	if (tiles[x + 1][y + 1].type == tileType) { neighbours += 1; };
	return neighbours;
}

void Map::drawFloorTile(int i, int j, sf::Color tileColor)
{
	int type = floorTilesRand[i][j];
	floorTiles[type]->setPosition(i * TILE_SIZE, j * TILE_SIZE);
	floorTiles[type]->setColor(tileColor);
	app->draw(*floorTiles[type]);
}

void Map::drawWallCornerTile(int i, int j, sf::Color tileColor)
{
	int type = wallCornerTilesRand[i][j];
	wallCornerTiles[type]->setPosition(i * TILE_SIZE, j * TILE_SIZE);
	wallCornerTiles[type]->setColor(tileColor);
	app->draw(*wallCornerTiles[type]);
}

void Map::drawWallHorizontalTile(int i, int j, sf::Color tileColor)
{
	int type = wallHorizontalTilesRand[i][j];
	wallHorizontalTiles[type]->setPosition(i * TILE_SIZE, j * TILE_SIZE);
	wallHorizontalTiles[type]->setColor(tileColor);
	app->draw(*wallHorizontalTiles[type]);
}

void Map::drawWallVerticalTile(int i, int j, sf::Color tileColor)
{
	int type = wallVerticalTilesRand[i][j];
	wallVerticalTiles[type]->setPosition(i * TILE_SIZE, j * TILE_SIZE);
	wallVerticalTiles[type]->setColor(tileColor);
	app->draw(*wallVerticalTiles[type]);
}

void Map::drawWallFillTile(int i, int j, sf::Color tileColor)
{
	int type = wallFillTilesRand[i][j];
	wallFillTiles[type]->setPosition(i * TILE_SIZE, j * TILE_SIZE);
	wallFillTiles[type]->setColor(tileColor);
	app->draw(*wallFillTiles[type]);
}

void Map::drawLavaFrame(int i, int j, sf::Color tileColor, float frameDuration)
{
	int frame = getCorrectFrame(lavaFrames.size(), frameDuration);
	lavaFrames[frame]->setPosition(i * TILE_SIZE, j * TILE_SIZE);
	lavaFrames[frame]->setColor(tileColor);
	app->draw(*lavaFrames[frame]);
}

/*
 * When in debug mode causes significant slowdowns.
 * Running the program in release mode seems to work
 * fine. Vectors in debug mode have extra checks for
 * safety.
 */
void Map::drawTile(std::vector<sf::Sprite *> tileVector, int tileVectorRand[MAP_SIZE_X][MAP_SIZE_Y], int i, int j, sf::Color tileColor)
{
	int type = tileVectorRand[i][j];
	tileVector[type]->setPosition(i * TILE_SIZE, j * TILE_SIZE);
	tileVector[type]->setColor(tileColor);
	app->draw(*tileVector[type]);
}

int Map::getCorrectFrame(int totFrames, float duration)
{
	int frame = 0;
	if (int((lastClockTmp + frameClock) / duration) > int(lastClockTmp / duration)) {
		/* Calculate the frame number */
		frame = int((lastClockTmp + frameClock) / duration);
		/* Adjust for looping */
		frame %= totFrames;
	} else {
		frame = int(lastClockTmp / duration);
		frame %= totFrames;
	}
	return frame;
}
