/*
 * Project Name
 * 2015 © Project Team (see: LICENSE)
 */

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
#define TILE_SIZE 16
#define MAP_SIZE_X 50
#define MAP_SIZE_Y 38

enum MapTileType
{
	mtAir,
	mtWall,
	mtForest,
	mtSolid
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
struct Map
{
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
	sf::Vector2f getTilePos(int x, int y);
	sf::Color getTileLight(int x, int y);

	/* Tiles */
	sf::Texture *bgTex;
	sf::Sprite *bgSpr;
	sf::Texture *tileTex;
	sf::Sprite *tileSpr;
	sf::Texture *forestTex;
	sf::Sprite *forestSpr;
	char ambientIntensity;
	sf::Color ambientColor;
	std::vector<StaticLightSource *> sources;
	MapTile tiles[MAP_SIZE_X][MAP_SIZE_Y];
	 Map();
	~Map();
	void update(StaticLightSource *tmpSource);
	void checkSources(StaticLightSource *tmpSource);
	void render();
	void renderTiles();
	void clear() {
		deleteList(sources);
	};
};

#endif
