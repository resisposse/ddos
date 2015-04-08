/*
 * Project Name
 * 2015 © Project Team (see: LICENSE)
 */

#include "fog.hpp"
#ifndef MAP
#define MAP

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
#define CORRIDOR_CHANCE 25
#define ROOM_CHANCE 75

enum MapTileType
{
	mtAir,
	mtWall,
	mtFloorMetal,
	mtFloor
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
 * The fog of war effect is very much integrated into drawing the map
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
	int checkNeighbourType(MapTile tile, MapTileType tileType);
	sf::Vector2f getTilePos(int x, int y);
	sf::Color getTileLight(int x, int y);

	/* Tiles */
	sf::Texture *tileMapTex;

	sf::Texture *bgTex;
	sf::Sprite *bgSpr;
	sf::Sprite *floorSpr;
	sf::Sprite *floorMetalSpr;
	sf::Sprite *floorDamagedV1Spr;
	sf::Sprite *floorDamagedV2Spr;
	sf::Sprite *floorDamagedV3Spr;
	sf::Sprite *floorDamagedV4Spr;
	sf::Sprite *floorDamagedV5Spr;

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

	void drawFloorTile(int x, int y, sf::Color tileColor);
	void drawWallCornerTile(int x, int y, sf::Color tileColor);
	void drawWallHorizontalTile(int x, int y, sf::Color tileColor);
	void drawWallVerticalTile(int x, int y, sf::Color tileColor);
	void drawWallFillTile(int x, int y, sf::Color tileColor);
	void drawTile(std::vector<sf::Sprite *> tileVector, int tileVectorRand[MAP_SIZE_X][MAP_SIZE_Y], int x, int y, sf::Color tileColor);

	long oldseed;

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
	void render();
	void renderTiles();
	int testCollision(int x, int y);
	int Collision(float X, float Y);
	void clear() {
		deleteList(sources);
	};
};

#endif
