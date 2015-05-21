/*
 * Dark Domains Of Space
 * 2015 © Project Team (see: LICENSE)
 */

#ifndef MAP
#define MAP

#include "globals.hpp"

enum MapTileType
{
	mtAir,
	mtWall,
	mtWallVoid,
	mtFloorMetal,
	mtFloor,
	mtLava,
	mtSpawn,
	mtGoal,
	mtDoor
};

class MapTile
{
public:
	MapTileType type;
	sf::Color light;
	sf::Vector2i index;
	char intensity;
	char absorb;
};

class Map
{
public:
	int direction;
	int collisionMap[MAP_SIZE_X][MAP_SIZE_Y];
	int floorTilesRand[MAP_SIZE_X][MAP_SIZE_Y];
	int wallHorizontalTilesRand[MAP_SIZE_X][MAP_SIZE_Y];
	int wallVerticalTilesRand[MAP_SIZE_X][MAP_SIZE_Y];
	int wallCornerTilesRand[MAP_SIZE_X][MAP_SIZE_Y];
	int wallFillTilesRand[MAP_SIZE_X][MAP_SIZE_Y];
	std::vector<sf::Sprite *> floorTiles;
	std::vector<sf::Sprite *> wallHorizontalTiles;
	std::vector<sf::Sprite *> wallVerticalTiles;
	std::vector<sf::Sprite *> wallCornerTiles;
	std::vector<sf::Sprite *> wallFillTiles;
	std::vector<sf::Sprite *> lavaFrames;
	MapTile tiles[MAP_SIZE_X][MAP_SIZE_Y];

	Map(char *generatedMap);
	~Map();
	int updateWallDirection(MapTile tile);
	int checkNeighbourType(MapTile tile, MapTileType tileType);
	int collision(float X, float Y, std::string collisionType);
	int getCorrectFrame(int totalFrames, float duration);
	void renderTiles();
	void drawFloorTile(int x, int y, sf::Color tileColor);
	void drawWallCornerTile(int x, int y, sf::Color tileColor);
	void drawWallHorizontalTile(int x, int y, sf::Color tileColor);
	void drawWallVerticalTile(int x, int y, sf::Color tileColor);
	void drawWallFillTile(int x, int y, sf::Color tileColor);
	void drawLavaFrame(int x, int y, sf::Color tileColor, float frameDuration);
	void drawTile(std::vector<sf::Sprite *> tileVector,
	              int tileVectorRand[MAP_SIZE_X][MAP_SIZE_Y],
	              int x, int y,
	              sf::Color tileColor);

	/* Tiles */
	sf::Texture *tileMapTex;
	sf::Texture *teleSpawnTex;
	sf::Texture *teleGoalTex;
	sf::Texture *lavaTex;
	sf::Texture *bgTex;
	sf::Sprite *bgSpr;
	sf::Sprite *floorSpr;
	sf::Sprite *floorSprCpy;
	sf::Sprite *floorMetalSpr;
	sf::Sprite *floorGrateHorSpr;
	sf::Sprite *floorGrateVerSpr;
	sf::Sprite *floorGrateTopSpr;
	sf::Sprite *floorGrateTopEmptySpr;
	sf::Sprite *floorGrateBottomEmptySpr;
	sf::Sprite *floorGrateLeftSpr;
	sf::Sprite *floorGrateTopRightSpr;
	sf::Sprite *floorDamagedV1Spr;
	sf::Sprite *floorDamagedV2Spr;
	sf::Sprite *floorDamagedV3Spr;
	sf::Sprite *floorDamagedV4Spr;
	sf::Sprite *floorDamagedV5Spr;
	sf::Sprite *floorVar1Spr;
	sf::Sprite *floorVar2Spr;
	sf::Sprite *floorVar3Spr;
	sf::Sprite *floorVar4Spr;
	sf::Sprite *floorVar5Spr;
	sf::Sprite *floorVar6Spr;
	sf::Sprite *floorVar7Spr;
	sf::Sprite *floorVar8Spr;
	sf::Sprite *floorVar9Spr;
	sf::Sprite *floorVar10Spr;
	sf::Sprite *floorVar11Spr;
	sf::Sprite *floorVar12Spr;
	sf::Sprite *floorVar13Spr;
	sf::Sprite *floorVar14Spr;
	sf::Sprite *floorTopLeftSpr;
	sf::Sprite *floorTopSpr;
	sf::Sprite *floorTopRightSpr;
	sf::Sprite *floorLeftSpr;
	sf::Sprite *floorRightSpr;
	sf::Sprite *floorBottomLeftSpr;
	sf::Sprite *floorBottomSpr;
	sf::Sprite *floorBottomRightSpr;
	sf::Sprite *floorTopLeftCornerSpr;
	sf::Sprite *floorTopRightCornerSpr;
	sf::Sprite *floorBottomLeftCornerSpr;
	sf::Sprite *floorBottomRightCornerSpr;

	sf::Sprite *wallMiddleSpr;

	sf::Sprite *wallHorizontal1Spr;
	sf::Sprite *wallHorizontal2Spr;
	sf::Sprite *wallHorizontal3Spr;
	sf::Sprite *wallHorizontal4Spr;
	sf::Sprite *wallHorizontal5Spr;
	sf::Sprite *wallVertical1Spr;
	sf::Sprite *wallVertical2Spr;
	sf::Sprite *wallVertical3Spr;
	sf::Sprite *wallVertical4Spr;
	sf::Sprite *wallVertical5Spr;
	sf::Sprite *wallCorner1Spr;
	sf::Sprite *wallCorner2Spr;
	sf::Sprite *wallCorner3Spr;
	sf::Sprite *wallCorner4Spr;
	sf::Sprite *wallFill1Spr;
	sf::Sprite *wallFill2Spr;
	sf::Sprite *wallFill3Spr;
	sf::Sprite *wallFill4Spr;
	sf::Sprite *wallFill5Spr;
	sf::Sprite *voidSpr;

	sf::Sprite *teleSpawnFullClosedSpr;
	sf::Sprite *teleGoalFullClosedSpr;
	sf::Sprite *teleDoorHorTopShadowSpr;
	sf::Sprite *teleDoorHorTopSpr;
	sf::Sprite *teleDoorVerLeftShadowSpr;
	sf::Sprite *teleDoorVerLeftSpr;
	sf::Sprite *teleDoorHorBottomShadowSpr;
	sf::Sprite *teleDoorHorBottomSpr;
	sf::Sprite *teleDoorVerRightShadowSpr;
	sf::Sprite *teleDoorVerRightSpr;

	sf::Sprite *lava1Spr;
	sf::Sprite *lava2Spr;
	sf::Sprite *lava3Spr;
	sf::Sprite *lava4Spr;
	sf::Sprite *lava5Spr;
	sf::Sprite *lava6Spr;
	sf::Sprite *lava7Spr;
	sf::Sprite *lava8Spr;
};

#endif
