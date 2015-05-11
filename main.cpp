/*
 * Dark Domains Of Space
 * 2015 © Project Team (see: LICENSE)
 */

#include <SFML/Graphics.hpp>
#include <iostream>
#include "light.hpp"
#include "map.hpp"
#include "mapgenerator.hpp"
#include "object.hpp"
#include "main.hpp"
#include "projectile.hpp"

float frameClock = 0;
float lastClockTmp = 0;
GameState state;
sf::RenderWindow *app;
Game *game;
sf::Clock timer;

/* Object.update takes type Time, change to use float frameCLock */
//sf::Time timeSinceLastUpdate = sf::Time::Zero;
const sf::Time Game::TimePerFrameTmp = sf::seconds(1.f / 60.f);

Game::Game()
{
	app = new sf::RenderWindow(sf::VideoMode(800, 608, 32),
	                           "Dark Domains Of Space",
	                           sf::Style::Resize | sf::Style::Close);
	running = true;
	lastClock = timer.getElapsedTime().asMilliseconds();

	loadCharacterTextures();
	loadCursorTexture();
	loadProjectileTextures();

	mapGenerator = new MapGenerator;
	map = new Map(mapGenerator->generateMap());
	light = new Light();
	player = new Player(*playerTexture);

	app->setFramerateLimit(60);
	initializeLighting();
	initializeView();
	spawnEnemies(5);
}

Game::~Game()
{
	delete map;
	delete mapGenerator;
	delete light;
	delete bulletTexture;
	delete laserBeamTexture;
	delete playerTexture;
	delete enemyMeleeTexture;
	delete player;
	delete app;
	delete playerView;
}

void Game::update()
{
	while (running) {
		currentClock += timer.getElapsedTime().asMilliseconds();
		frameClock = (currentClock - lastClock) / 1000.f;
		lastClockTmp = lastClock / 1000.f;
		lastClock = currentClock;

		parseEvents();
		player->run();
		app->clear();

		updateLighting();
		updateProjectiles();
		updateEnemies();
		checkProjectileCollisions();

		render();

		playerView->setCenter(playerPositionX, playerPositionY);
		map->bgSpr->setOrigin(400, 300);
		map->bgSpr->setPosition(playerPositionX, playerPositionY);
		app->setView(*playerView);
		app->display();
	}
}

void Game::render()
{
	light->initialize();
	map->renderTiles();
	light->update(&state.tmpSource);

	drawPlayer();
	drawProjectiles();
	drawEnemies();
	drawCursor();
}

void Game::initializeLighting()
{
	state.brush.type = stStatic;
	state.brush.intensity = 100;
	state.brush.color = sf::Color::Red;
	state.brush.color.r = 150;
	state.brush.color.g = 150;
	state.brush.color.b = 150;
	state.brush.sourceTime = 2.0f;
	state.ambientIntensity = 0;
	state.ambientColor = sf::Color::White;
	state.ambientColor.r = 0;
	state.ambientColor.g = 0;
	state.ambientColor.b = 0;

	/* Options are: stStatic, stPulsing, stFading, stTest */
	state.brush.type = stPulsing;
}

void Game::initializeView()
{
	playerPositionX = player->sprite.getPosition().x;
	playerPositionY = player->sprite.getPosition().y;

	this->zoomLevel = 1.0f;
	playerView = new sf::View;
	playerView->setCenter(playerPositionX, playerPositionY);
	playerView->setSize(sf::Vector2f(800, 608));
	app->setView(*playerView);
}

/*
 * Keep realigning the center of the light (fog of war) with the player and then
 * reread the light's attributes on the off chance that they were changed.
 */
void Game::updateLighting()
{
	playerPositionX = player->sprite.getPosition().x;
	playerPositionY = player->sprite.getPosition().y;
	state.brush.position = sf::Vector2i((int)playerPositionX / TILE_SIZE,
	                                    (int)playerPositionY / TILE_SIZE);
	state.tmpSource = StaticLightSource(state.brush.position,
	                                    state.brush.color,
	                                    state.brush.intensity);
	light->ambientColor = state.ambientColor;
	light->ambientIntensity = state.ambientIntensity;
}

/*
 * A lot of events are handled separately and thus we have numerous event
 * processing functions of the same name. It might or might not be easier to
 * handle this way.
 *
 * http://bit.ly/1niVeJF
 */
void Game::parseEvents()
{
	sf::Event event;
	while (app->pollEvent(event)) {
		game->processEvent(event);
		player->processEvent(event);
	}
	if (!app->isOpen()) {
		running = false;
	}
}

void Game::processEvent(sf::Event event)
{
	switch(event.type) {
	case sf::Event::MouseWheelMoved: {
		if (event.mouseWheel.delta < 0) {
			if (zoomLevel < 2.0f) {
				playerView->zoom(2.0f);
				zoomLevel *= 2.0f;
			}
		} else {
			if (zoomLevel > 0.25f) {
				playerView->zoom(0.5f);
				zoomLevel *= 0.5f;
			}
		}
		break;
	}
	case sf::Event::KeyPressed: {
		if (event.key.code == sf::Keyboard::Escape) {
			app->close();
			break;
		} else if (event.key.code == sf::Keyboard::E) {
			ammoType = (ammoType + 1) % 2;
		} else if (event.key.code == sf::Keyboard::Q) {
			ammoType = abs((ammoType - 1) % 2);
		}
		break;
	}
	case sf::Event::MouseButtonPressed: {
		if (event.mouseButton.button == sf::Mouse::Left) {
			if (ammoType == 0) {
				projectiles.push_back(BulletSprite(*bulletTexture, player->sprite.getPosition(), sf::Vector2i(app->mapPixelToCoords(sf::Mouse::getPosition(*app)))));
			} else if (ammoType == 1) {
				projectiles.push_back(LaserSprite(*laserBeamTexture, player->sprite.getPosition(), sf::Vector2i(app->mapPixelToCoords(sf::Mouse::getPosition(*app)))));
			} else {
				std::cout << "AmmoType fail: " << ammoType << std::endl;
			}
		}
		break;
	}
	case sf::Event::MouseButtonReleased: {
		//if (event.mouseButton.button == sf::Mouse::Left) addSource();
		if (event.mouseButton.button == sf::Mouse::Right) light->clear();
		break;
	}
	case sf::Event::Closed: {
		running = false;
		break;
	}
	case sf::Event::Resized: {
		playerView->setSize(event.size.width, event.size.height);
		map->bgSpr->setPosition(app->mapPixelToCoords(sf::Vector2i(0, 0), *playerView));
		sf::Vector2f pos = sf::Vector2f(event.size.width, event.size.height);
		pos *= 0.5f;
		pos = app->mapPixelToCoords(sf::Vector2i(pos), *playerView);
		map->bgSpr->setScale(
			float(event.size.width) / float(map->bgSpr->getTexture()->getSize().x),
			float(event.size.height) / float(map->bgSpr->getTexture()->getSize().y));
		break;
	}
	default: {
		break;
	}
	}
}

void Game::addSource()
{
	switch (state.brush.type) {
	case stStatic:
		light->sources.push_back((StaticLightSource *)
		                       (new StaticLightSource
		                       (state.brush.position,
		                        state.brush.color,
		                        state.brush.intensity)));
		break;
	case stFading:
		light->sources.push_back((StaticLightSource *)
		                       (new FadingLightSource
		                       (state.brush.position,
		                        state.brush.color,
		                        state.brush.intensity,
		                        state.brush.sourceTime)));
		break;
	case stPulsing:
		light->sources.push_back((StaticLightSource *)
		                       (new PulsingLightSource
		                       (state.brush.position,
		                        state.brush.color,
		                        state.brush.intensity,
		                        state.brush.sourceTime)));
		break;
	case stTest:
		light->sources.push_back((StaticLightSource *)
		                       (new TestLightSource
		                       (state.brush.position,
		                        state.brush.color,
		                        state.brush.intensity,
		                        state.brush.sourceTime)));
		break;
	}
}

int Game::collision(float x, float y, std::string collisionType)
{
	int i = map->collision(x, y, collisionType);
	return i;
}

void Game::loadCharacterTextures()
{
	playerTexture = new sf::Texture();
	playerTexture->loadFromFile("media/ddos-dude-guns.png");
	playerTexture->setSmooth(true);

	enemyMeleeTexture = new sf::Texture();
	enemyMeleeTexture->loadFromFile("media/ddos-dude-guns.png");
	enemyMeleeTexture->setSmooth(true);
}

void Game::loadCursorTexture()
{
	app->setMouseCursorVisible(false);
	fixed = app->getView();

	TextureCursor = new sf::Texture();
	TextureCursor->loadFromFile("media/cursor.png");
	spriteCursor = new sf::Sprite(*TextureCursor);
	sf::Vector2u spriteSize = TextureCursor->getSize();
	spriteCursor->setOrigin(spriteSize.x / 2, spriteSize.y / 2);
	spriteCursor->setColor(sf::Color(255, 0, 0, 255));
	app->setView(fixed);
}

void Game::loadProjectileTextures()
{
	bulletTexture = new sf::Texture();
	bulletTexture->loadFromFile("media/bullet.png");
	bulletTexture->setSmooth(true);

	laserBeamTexture = new sf::Texture();
	laserBeamTexture->loadFromFile("media/laserBeam.png");
	laserBeamTexture->setSmooth(true);
}

void Game::updateProjectiles()
{
	for (unsigned int i = 0; i < projectiles.size();) {
		projectiles[i].update(frameClock);
		i++;
	}
}

void Game::checkProjectileCollisions()
{
	for (unsigned int i = 0; i < projectiles.size(); i++) {
		int x, y;
		x = projectiles[i].position.x;
		y = projectiles[i].position.y;
		if (collision(x, y, "projectile") == 1) {
			projectiles.erase(projectiles.begin() + i);
			break;
		}
	}
}

void Game::drawProjectiles()
{
	for (ProjectileSprite &projectile : projectiles) {
		app->draw(projectile.sprite);
	}
}

void Game::spawnEnemies(int amount)
{
	for (int i = 0; i < amount; i++) {
		enemies.push_back(EnemyMelee(*enemyMeleeTexture));
		std::cout << "Enemy Spawned" << std::endl;
	}
}

void Game::updateEnemies()
{
	for (unsigned int i = 0; i <  enemies.size();) {
		enemies[i].update(enemies[i].sprite.getPosition().x,
		                  enemies[i].sprite.getPosition().y,
		                  player->sprite.getPosition().x,
		                  player->sprite.getPosition().y);
		i++;
	}
}

void Game::drawEnemies()
{
	for (unsigned int i = 0; i < enemies.size(); i++) {
		app->draw(enemies[i].sprite);
	}
}

/*
void Game::updatePlayer()
{
	player->update(TimePerFrameTmp);
}
*/

void Game::drawPlayer()
{
	app->draw(player->sprite);
}

void Game::drawCursor()
{
	mouse = sf::Vector2i(app->mapPixelToCoords(sf::Mouse::getPosition(*app)));
	spriteCursor->setPosition(static_cast<sf::Vector2f>(mouse));
	app->draw(*spriteCursor);
}

int main()
{
	game = new Game;
	srand(time(NULL));
	while (game->running) {
		game->update();
	}
	delete game;
	return EXIT_SUCCESS;
}
