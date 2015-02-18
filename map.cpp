/*
 * Project Name
 * 2015 © Project Team (see: LICENSE)
 */

#include <SFML/Graphics.hpp>
#include "fog.hpp"
#include "map.hpp"
#include "main.hpp"

Map::Map()
{
	bgTex = new sf::Texture();
	bgSpr = new sf::Sprite();
	bgTex->loadFromFile("media/background.png");
	bgSpr->setTexture(*bgTex);
	tileTex = new sf::Texture();
	tileSpr = new sf::Sprite();
	tileTex->loadFromFile("media/grass.png");
	tileSpr->setTexture(*tileTex);
	forestTex = new sf::Texture();
	forestSpr = new sf::Sprite();
	forestTex->loadFromFile("media/forest.png");
	forestSpr->setTexture(*forestTex);
	char _tiles[MAP_SIZE_X][MAP_SIZE_Y + 1] = {
		"                                  XXXX",
		"                                 XXXXX",
		"                                XXXXXX",
		"                           XXXXXXXXXXX",
		"                           XXXXXXXXXXX",
		"                          XX.......XXX",
		"XXXXXXXXX                 XX.......XXX",
		"XXXXXXXXX                XXX.......XXX",
		"X......XX                XXX.......XXX",
		"X......XXX              XXXX.......XXX",
		"X.......XXX             XXXX.......XXX",
		"X........XXX            XXXX.......XXX",
		"X.........XX            XXXX.......XXX",
		"X.........XX             XXX.......XXX",
		"X.........XX    XXXXXXXXXXXX.......XXX",
		"X.........XX    XXXXXXXXXXXX.......XXX",
		"X.........XX    XXXXXXXXXXXX.......XXX",
		"X.........XXX   XXX....XXXXXXX.....XXX",
		"X.........XXX   XXX....XXXXXXX.....XXX",
		"X000000000XXX   XXX....XXXXXXX.....XXX",
		"X...00....XXXX  XXX....XXXXXXX.....XXX",
		"X...000...XXXX  XXX....XXXXXXX.....XXX",
		"X...00....XXXX  XXX....XXXXXXX.....XXX",
		"X...000...XXXXXXXXX................XXX",
		"X...00....XXXXXXXXX................XXX",
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
		tiles[i][j].index = sf::Vector2i(i, j);
		switch (_tiles[i][j]) {
		case 'X':
			tiles[i][j].type = mtSolid;
			tiles[i][j].absorb = 35;
			break;
		case '.':
			tiles[i][j].type = mtWall;
			tiles[i][j].absorb = 6;
			break;
		case ' ':
			tiles[i][j].type = mtAir;
			tiles[i][j].absorb = 35;
			break;
		case '0':
			tiles[i][j].type = mtForest;
			tiles[i][j].absorb = 6;
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
	delete tileTex;
	delete tileSpr;
	delete forestTex;
	delete forestSpr;
	deleteList(sources);
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
			tileColor = sf::Color(140, 140, 140, 255);
		case mtSolid:
			tileSpr->setPosition(i * TILE_SIZE, j * TILE_SIZE);
			tileSpr->setColor(tileColor);
			app->draw(*tileSpr);
			break;
		case mtForest:
			forestSpr->setPosition(i * TILE_SIZE, j * TILE_SIZE);
			forestSpr->setColor(tileColor);
			app->draw(*forestSpr);
			break;
		case mtAir:
			break;
		}
	}
}
