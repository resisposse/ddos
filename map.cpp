/*
* Project Name
* 2015 © Project Team (see: LICENSE)
*/

#include <SFML/Graphics.hpp>
#include "fog.hpp"
#include "map.hpp"
#include "main.hpp"

#include <vector>
#include <array>
#include <list>

Map::Map()
{
	bgTex = new sf::Texture();
	bgSpr = new sf::Sprite();
	bgTex->loadFromFile("media/background.png");
	bgSpr->setTexture(*bgTex);

	floorTex = new sf::Texture();
	floorSpr = new sf::Sprite();
	floorTex->loadFromFile("media/floor_16px.png");
	floorSpr->setTexture(*floorTex);
	floorMetalTex = new sf::Texture();
	floorMetalSpr = new sf::Sprite();
	floorMetalTex->loadFromFile("media/floor_metal_16px.png");
	floorMetalSpr->setTexture(*floorMetalTex);

	wallTopLeftTex = new sf::Texture();
	wallTopLeftSpr = new sf::Sprite();
	wallTopLeftTex->loadFromFile("media/wall_top_left_16px.png");
	wallTopLeftSpr->setTexture(*wallTopLeftTex);
	wallTopTex = new sf::Texture();
	wallTopSpr = new sf::Sprite();
	wallTopTex->loadFromFile("media/wall_top_16px.png");
	wallTopSpr->setTexture(*wallTopTex);
	wallTopRightTex = new sf::Texture();
	wallTopRightSpr = new sf::Sprite();
	wallTopRightTex->loadFromFile("media/wall_top_right_16px.png");
	wallTopRightSpr->setTexture(*wallTopRightTex);
	wallLeftTex = new sf::Texture();
	wallLeftSpr = new sf::Sprite();
	wallLeftTex->loadFromFile("media/wall_left_16px.png");
	wallLeftSpr->setTexture(*wallLeftTex);
	wallMiddleTex = new sf::Texture();
	wallMiddleSpr = new sf::Sprite();
	wallMiddleTex->loadFromFile("media/wall_middle_16px.png");
	wallMiddleSpr->setTexture(*wallMiddleTex);
	wallRightTex = new sf::Texture();
	wallRightSpr = new sf::Sprite();
	wallRightTex->loadFromFile("media/wall_right_16px.png");
	wallRightSpr->setTexture(*wallRightTex);
	wallBottomLeftTex = new sf::Texture();
	wallBottomLeftSpr = new sf::Sprite();
	wallBottomLeftTex->loadFromFile("media/wall_bottom_left_16px.png");
	wallBottomLeftSpr->setTexture(*wallBottomLeftTex);
	wallBottomTex = new sf::Texture();
	wallBottomSpr = new sf::Sprite();
	wallBottomTex->loadFromFile("media/wall_bottom_16px.png");
	wallBottomSpr->setTexture(*wallBottomTex);
	wallBottomRightTex = new sf::Texture();
	wallBottomRightSpr = new sf::Sprite();
	wallBottomRightTex->loadFromFile("media/wall_bottom_right_16px.png");
	wallBottomRightSpr->setTexture(*wallBottomRightTex);

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
			tiles[i][j].type = mtWall;
			tiles[i][j].absorb = 35;
			break;
		case '.':
			tiles[i][j].type = mtFloor;
			tiles[i][j].absorb = 6;
			break;
		case ' ':
			tiles[i][j].type = mtAir;
			tiles[i][j].absorb = 35;
			break;
		case '0':
			tiles[i][j].type = mtFloorMetal;
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

	delete floorTex;
	delete floorSpr;
	delete floorMetalTex;
	delete floorMetalSpr;

	delete wallTopLeftTex;
	delete wallTopLeftSpr;
	delete wallTopTex;
	delete wallTopSpr;
	delete wallTopRightTex;
	delete wallTopRightSpr;
	delete wallLeftTex;
	delete wallLeftSpr;
	delete wallMiddleTex;
	delete wallMiddleSpr;
	delete wallRightTex;
	delete wallRightSpr;
	delete wallBottomLeftTex;
	delete wallBottomLeftSpr;
	delete wallBottomTex;
	delete wallBottomSpr;
	delete wallBottomRightSpr;
	delete wallBottomRightTex;

	deleteList(sources);
}
/* TODO Add more types of wall types, like T-shaped etc. */
int Map::updateWallDirection(MapTile tile)
{
	/* Tile positions
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
				wallTopLeftSpr->setPosition(i * TILE_SIZE, j * TILE_SIZE);
				wallTopLeftSpr->setColor(tileColor);
				app->draw(*wallTopLeftSpr);
				break;
			case 2:
				wallTopSpr->setPosition(i * TILE_SIZE, j * TILE_SIZE);
				wallTopSpr->setColor(tileColor);
				app->draw(*wallTopSpr);
				break;
			case 3:
				wallTopRightSpr->setPosition(i * TILE_SIZE, j * TILE_SIZE);
				wallTopRightSpr->setColor(tileColor);
				app->draw(*wallTopRightSpr);
				break;
			case 4:
				wallLeftSpr->setPosition(i * TILE_SIZE, j * TILE_SIZE);
				wallLeftSpr->setColor(tileColor);
				app->draw(*wallLeftSpr);
				break;
			case 5:
				wallMiddleSpr->setPosition(i * TILE_SIZE, j * TILE_SIZE);
				wallMiddleSpr->setColor(tileColor);
				app->draw(*wallMiddleSpr);
				break;
			case 6:
				wallRightSpr->setPosition(i * TILE_SIZE, j * TILE_SIZE);
				wallRightSpr->setColor(tileColor);
				app->draw(*wallRightSpr);
				break;
			case 7:
				wallBottomLeftSpr->setPosition(i * TILE_SIZE, j * TILE_SIZE);
				wallBottomLeftSpr->setColor(tileColor);
				app->draw(*wallBottomLeftSpr);
				break;
			case 8:
				wallBottomSpr->setPosition(i * TILE_SIZE, j * TILE_SIZE);
				wallBottomSpr->setColor(tileColor);
				app->draw(*wallBottomSpr);
				break;
			case 9:
				wallBottomRightSpr->setPosition(i * TILE_SIZE, j * TILE_SIZE);
				wallBottomRightSpr->setColor(tileColor);
				app->draw(*wallBottomRightSpr);
				break;
			}
			break;
		case mtFloor:
			floorSpr->setPosition(i * TILE_SIZE, j * TILE_SIZE);
			floorSpr->setColor(tileColor);
			app->draw(*floorSpr);
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