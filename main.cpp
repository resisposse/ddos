/*
 * Project Name
 * 2015 ? Project Team (see: LICENSE)
 */

#include <SFML/Graphics.hpp>
#include "fog.hpp"
#include "map.hpp"
#include "mapgenerator.hpp"
#include "object.hpp"
#include "main.hpp"
#include "projectileSprite.hpp"
#include <iostream>

float frameClock = 0;
float lastClockTmp = 0;
GameState state;
sf::RenderWindow *app;
sf::View *playerView;
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
	player = new Player;
	mapGenerator = new MapGenerator;
	map = new Map(mapGenerator->generateMap());
	app->setFramerateLimit(60);
	state.brush.type = stStatic;
	state.brush.color = sf::Color::Red;
	state.brush.intensity = LIGHT_MAX_LIGHTLEVEL;
	state.brush.sourceTime = 2.0f;
	state.ambientColor = sf::Color::White;
	state.ambientIntensity = 5;

	loadCursorTexture();
	loadProjectileTextures();

	spawnEnemies(5);

	this->zoomLevel = 1.0f;
	playerView = new sf::View;

	float positionPlayerX = player->sprite.getPosition().x;
	float positionPlayerY = player->sprite.getPosition().y;

	playerView->setCenter(positionPlayerX, positionPlayerY);
	playerView->setSize(sf::Vector2f(800, 608));
	app->setView(*playerView);
}

Game::~Game()
{
	delete map;
	delete mapGenerator;
	delete bulletTexture;
	delete laserBeamTexture;
	delete player;
	delete app;
	delete playerView;
}

int Game::collision(float x, float y, std::string collisionType) {
	int i = map->Collision(x, y, collisionType);
	return i;
}

void Game::update()
{
	state.ambientIntensity = 0;
	state.ambientColor.r = 0;
	state.ambientColor.g = 0;
	state.ambientColor.b = 0;
	state.brush.intensity = 100;
	state.brush.color.r = 150;
	state.brush.color.g = 150;
	state.brush.color.b = 150;
	state.brush.sourceTime = 2.0f;

	/* Options are: stStatic, stPulsing, stFading, stTest */
	state.brush.type = stPulsing;

	while (running) {
		currentClock += timer.getElapsedTime().asMilliseconds();
		frameClock = (currentClock - lastClock) / 1000.f;
		lastClockTmp = lastClock / 1000.f;
		lastClock = currentClock;

		float positionX = player->sprite.getPosition().x;
		float positionY = player->sprite.getPosition().y;
		
		state.brush.position = sf::Vector2i((int)positionX / TILE_SIZE,
		                                    (int)positionY / TILE_SIZE);
		state.tmpSource = StaticLightSource(state.brush.position,
		                                    state.brush.color,
		                                    state.brush.intensity);
		map->ambientColor = state.ambientColor;
		map->ambientIntensity = state.ambientIntensity;

		processEvents();
		player->run();
		app->clear();

		updateProjectiles();
		checkProjectileCollisions();
		updateEnemies();

		render();
		
		playerView->setCenter(positionX, positionY);
		map->bgSpr->setOrigin(400, 300);
		map->bgSpr->setPosition(positionX, positionY);
		app->setView(*playerView);
		app->display();
	}
}

void Game::render()
{
	map->update(&state.tmpSource);
	drawPlayer();
	drawProjectiles();
	drawEnemies();
	drawCursor();
}

void Game::processEvents()
{
	sf::Event event;
	while (app->pollEvent(event)) {

		processEvent(event);
		player->processEvent(event);

		if (event.type == sf::Event::Resized)
		{
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
			//spawnEnemies(1);
			if (ammoType == 0) {
				projectiles.push_back(BulletSprite(*bulletTexture, player->sprite.getPosition(), sf::Vector2i(app->mapPixelToCoords(sf::Mouse::getPosition(*app)))));
			} else if (ammoType == 1) {
				projectiles.push_back(LaserSprite(*laserBeamTexture, player->sprite.getPosition(), sf::Vector2i(app->mapPixelToCoords(sf::Mouse::getPosition(*app)))));
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

void Game::spawnEnemies(int amount)
{
	for (int i = 0; i < amount; i++) {
		enemies.push_back(EnemyMelee());
		std::cout << "Enemy Spawned" << std::endl;
		//std::cout << enemies.size() << std::endl;
		//enemies.back->setPosition(rand() % 800, rand() % 600);
	}
}

void Game::updateEnemies()
{
	for (unsigned int i = 0; i <  enemies.size();) {
		enemies[i].update(TimePerFrameTmp, enemies[i].sprite.getPosition().x, enemies[i].sprite.getPosition().y,
						player->sprite.getPosition().x, player->sprite.getPosition().y);
		//std::cout << "UpdateEnemy: " << i << std::endl;
		++i;
	}
}

void Game::drawEnemies()
{
	for (Object &enemies : enemies) {
		app->draw(enemies.sprite);
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
	/* Draw custom cursor */
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
