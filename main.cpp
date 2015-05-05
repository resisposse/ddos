/*
 * Dark Domains Of Space
 * 2015 © Project Team (see: LICENSE)
 */

#include <iostream>
#include <SFML/Graphics.hpp>
#include "fog.hpp"
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

Game::Game()
{
	app = new sf::RenderWindow(sf::VideoMode(800, 608, 32),
	                           "Dark Domains Of Space",
	                           sf::Style::Resize | sf::Style::Close);
	running = true;
	lastClock = timer.getElapsedTime().asMilliseconds();

	player = new Player;
	enemy = new EnemyMelee;
	mapGenerator = new MapGenerator;
	map = new Map(mapGenerator->generateMap());

	app->setFramerateLimit(60);
	initializeLighting();
	initializeView();
	loadProjectileTextures();
}

Game::~Game()
{
	delete map;
	delete mapGenerator;
	delete bulletTexture;
	delete laserBeamTexture;
	delete player;
	delete enemy;
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

		refreshLighting();
		parseEvents();
		player->run();
		enemy->run();
		app->clear();
		render();
		updateProjectiles();
		checkProjectileCollisions();
		drawProjectiles();
		playerView->setCenter(playerPositionX, playerPositionY);
		app->setView(*playerView);
		map->bgSpr->setOrigin(400, 300);
		map->bgSpr->setPosition(playerPositionX, playerPositionY);
		app->display();
	}
}

void Game::render()
{
	map->update(&state.tmpSource);
	player->render();
	enemy->render();
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
	playerPositionX = mPlayerSpr->getPosition().x;
	playerPositionY = mPlayerSpr->getPosition().y;

	this->zoomLevel = 1.0f;
	playerView = new sf::View;
	playerView->setCenter(playerPositionX, playerPositionY);
	playerView->setSize(sf::Vector2f(800, 608));
	app->setView(*playerView);
}

/*
 * Keep realigning the center of the light (fog of war) with
 * the player and then reread the light's attributes on the
 * off chance that they were changed.
 */
void Game::refreshLighting()
{
	playerPositionX = mPlayerSpr->getPosition().x;
	playerPositionY = mPlayerSpr->getPosition().y;
	state.brush.position = sf::Vector2i((int)playerPositionX / TILE_SIZE,
	                                    (int)playerPositionY / TILE_SIZE);
	state.tmpSource = StaticLightSource(state.brush.position,
	                                    state.brush.color,
	                                    state.brush.intensity);
	map->ambientColor = state.ambientColor;
	map->ambientIntensity = state.ambientIntensity;
}

/*
 * A lot of events are handled separately and thus we have numerous
 * event processing functions of the same name. It might or might not
 * be easier to handle this way.
 *
 * http://bit.ly/1niVeJF
 */
void Game::parseEvents()
{
	sf::Event event;
	while (app->pollEvent(event)) {
		game->processEvent(event);
		player->processEvent(event);
		enemy->processEvent(event);
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
			ammoType = std::abs((ammoType - 1) % 2);
		}
		break;
	}
	case sf::Event::MouseButtonPressed: {
		if (event.mouseButton.button == sf::Mouse::Left)
			if (ammoType == 0) {
				projectiles.push_back(BulletSprite(*bulletTexture, mPlayerSpr->getPosition(), sf::Vector2i(app->mapPixelToCoords(sf::Mouse::getPosition(*app)))));
			} else if (ammoType == 1) {
				projectiles.push_back(LaserSprite(*laserBeamTexture, mPlayerSpr->getPosition(), sf::Vector2i(app->mapPixelToCoords(sf::Mouse::getPosition(*app)))));
			} else {
				std::cout << "AmmoType fail: " << ammoType << std::endl;
			}
		break;
	}
	case sf::Event::MouseButtonReleased: {
		//if (event.mouseButton.button == sf::Mouse::Left) addSource();
		if (event.mouseButton.button == sf::Mouse::Right) map->clear();
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
	}
}

void Game::addSource()
{
	switch (state.brush.type) {
	case stStatic:
		map->sources.push_back((StaticLightSource *)
		                       (new StaticLightSource
		                       (state.brush.position,
		                        state.brush.color,
		                        state.brush.intensity)));
		break;
	case stFading:
		map->sources.push_back((StaticLightSource *)
		                       (new FadingLightSource
		                       (state.brush.position,
		                        state.brush.color,
		                        state.brush.intensity,
		                        state.brush.sourceTime)));
		break;
	case stPulsing:
		map->sources.push_back((StaticLightSource *)
		                       (new PulsingLightSource
		                       (state.brush.position,
		                        state.brush.color,
		                        state.brush.intensity,
		                        state.brush.sourceTime)));
		break;
	case stTest:
		map->sources.push_back((StaticLightSource *)
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
	int i = map->Collision(x, y, collisionType);
	return i;
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
		++i;
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
