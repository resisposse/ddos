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
	tileMapTex = new sf::Texture();
	tileMapTex->loadFromFile("media/ddos-tiles1.png");

	bgTex = new sf::Texture();
	bgTex->loadFromFile("media/background.png");
	bgSpr = new sf::Sprite(*bgTex);

	sf::IntRect floor           (TILE_SIZE * 5, TILE_SIZE * 5, TILE_SIZE, TILE_SIZE);
	sf::IntRect floorMetal      (TILE_SIZE * 5, TILE_SIZE * 2, TILE_SIZE, TILE_SIZE);

	sf::IntRect wallTopLeft     (TILE_SIZE * 2, TILE_SIZE * 0, TILE_SIZE, TILE_SIZE);
	sf::IntRect wallTop         (TILE_SIZE * 4, TILE_SIZE * 0, TILE_SIZE, TILE_SIZE);
	sf::IntRect wallTopRight    (TILE_SIZE * 3, TILE_SIZE * 0, TILE_SIZE, TILE_SIZE);
	sf::IntRect wallLeft        (TILE_SIZE * 3, TILE_SIZE * 3, TILE_SIZE, TILE_SIZE);
	sf::IntRect wallMiddle      (TILE_SIZE * 2, TILE_SIZE * 1, TILE_SIZE, TILE_SIZE);
	sf::IntRect wallRight       (TILE_SIZE * 3, TILE_SIZE * 5, TILE_SIZE, TILE_SIZE);
	sf::IntRect wallBottomLeft  (TILE_SIZE * 3, TILE_SIZE * 1, TILE_SIZE, TILE_SIZE);
	sf::IntRect wallBottom      (TILE_SIZE * 5, TILE_SIZE * 0, TILE_SIZE, TILE_SIZE);
	sf::IntRect wallBottomRight (TILE_SIZE * 2, TILE_SIZE * 1, TILE_SIZE, TILE_SIZE);

	floorSpr           = new sf::Sprite(*tileMapTex, floor);
	floorMetalSpr      = new sf::Sprite(*tileMapTex, floorMetal);

	wallTopLeftSpr     = new sf::Sprite(*tileMapTex, wallTopLeft);
	wallTopSpr         = new sf::Sprite(*tileMapTex, wallTop);
	wallTopRightSpr    = new sf::Sprite(*tileMapTex, wallTopRight);
	wallLeftSpr        = new sf::Sprite(*tileMapTex, wallLeft);
	wallMiddleSpr      = new sf::Sprite(*tileMapTex, wallMiddle);
	wallRightSpr       = new sf::Sprite(*tileMapTex, wallRight);
	wallBottomLeftSpr  = new sf::Sprite(*tileMapTex, wallBottomLeft);
	wallBottomSpr      = new sf::Sprite(*tileMapTex, wallBottom);
	wallBottomRightSpr = new sf::Sprite(*tileMapTex, wallBottomRight);

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
	delete floorSpr;
	delete floorMetalSpr;
	delete wallTopLeftSpr;
	delete wallTopSpr;
	delete wallTopRightSpr;
	delete wallLeftSpr;
	delete wallMiddleSpr;
	delete wallRightSpr;
	delete wallBottomLeftSpr;
	delete wallBottomSpr;
	delete wallBottomRightSpr;
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
