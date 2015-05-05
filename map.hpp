/*
 * Dark Domains Of Space
 * 2015 © Project Team (see: LICENSE)
 */

#ifndef MAP
#define MAP

#include "fog.hpp"

#define deleteList(list) \
for (; !(list).empty(); delete (list).back(), (list).pop_back())
#define updateList(list) \
for (unsigned int i = 0; i < (list).size();) { \
	if (!(list)[i]->update()) { \
		delete (list)[i]; \
		(list).erase((list).begin() + i); \
		} else { \
		i++; \
		} \
}

#define TILE_SIZE 32
#define MAP_SIZE_X 50
#define MAP_SIZE_Y 38

enum MapTileType
{
	mtAir,
	mtWall,
	mtFloorMetal,
	mtFloor,
	mtLava
};

struct MapTile
{
	MapTileType type;
	sf::Color light;
	sf::Vector2i index;
	char intensity;
	char absorb;
};

/*
 * The Fog Of War effect is very much integrated into drawing the map
 * so we have some prototypes mixed in here. Editing the attributes
 * of FOW happens from 'map.cpp' while the actual implementations reside
 * in 'fog.cpp'.
 */
class Map
{
public:
	/* Lighting */
	MapTile **lightTiles[LIGHT_MAX_LIGHTLEVEL];
	int lightCounts[LIGHT_MAX_LIGHTLEVEL];
	sf::Vertex lightMask[4];
	void light();
	void resetLight();
	void buildLight();
	void renderLight();
	void setIntensity(MapTile *tile, char intensity, sf::Color color);
	void addIntensity(sf::Vector2i index, char intensity, sf::Color color);
	void initIntensity(MapTile *tile);
	void checkNeighbours(MapTile *tile);
	unsigned int checkNeighbourType(MapTile tile, MapTileType tileType);
	sf::Vector2f getTilePos(int x, int y);
	sf::Color getTileLight(int x, int y);

	/* Tiles */
	sf::Texture *tileMapTex;
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

	sf::Sprite *lava1Spr;
	sf::Sprite *lava2Spr;
	sf::Sprite *lava3Spr;
	sf::Sprite *lava4Spr;
	sf::Sprite *lava5Spr;
	sf::Sprite *lava6Spr;
	sf::Sprite *lava7Spr;
	sf::Sprite *lava8Spr;

	std::vector<sf::Sprite *> floorTiles;
	int floorTilesRand[MAP_SIZE_X][MAP_SIZE_Y];
	std::vector<sf::Sprite *> wallHorizontalTiles;
	int wallHorizontalTilesRand[MAP_SIZE_X][MAP_SIZE_Y];
	std::vector<sf::Sprite *> wallVerticalTiles;
	int wallVerticalTilesRand[MAP_SIZE_X][MAP_SIZE_Y];
	std::vector<sf::Sprite *> wallCornerTiles;
	int wallCornerTilesRand[MAP_SIZE_X][MAP_SIZE_Y];
	std::vector<sf::Sprite *> wallFillTiles;
	int wallFillTilesRand[MAP_SIZE_X][MAP_SIZE_Y];
	std::vector<sf::Sprite *> lavaFrames;
	int getCorrectFrame(int totFrames, float duration);

	void drawFloorTile          (int x, int y, sf::Color tileColor);
	void drawWallCornerTile     (int x, int y, sf::Color tileColor);
	void drawWallHorizontalTile (int x, int y, sf::Color tileColor);
	void drawWallVerticalTile   (int x, int y, sf::Color tileColor);
	void drawWallFillTile       (int x, int y, sf::Color tileColor);
	void drawLavaFrame          (int x, int y, sf::Color tileColor, float frameDuration);
	void drawTile               (std::vector<sf::Sprite *> tileVector, int tileVectorRand[MAP_SIZE_X][MAP_SIZE_Y], int x, int y, sf::Color tileColor);

	int direction;
	char ambientIntensity;
	sf::Color ambientColor;
	std::vector<StaticLightSource *> sources;
	MapTile tiles[MAP_SIZE_X][MAP_SIZE_Y];
	int collisionMap[MAP_SIZE_X][MAP_SIZE_Y];
	Map(char **generatedMap);
	~Map();
	int updateWallDirection(MapTile tile);
	void update(StaticLightSource *tmpSource);
	void checkSources(StaticLightSource *tmpSource);
	void renderTiles();
	int Collision(float X, float Y, std::string collisionType);
	void clear() {
		deleteList(sources);
	};
};

#endif
