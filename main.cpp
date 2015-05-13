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
#include "projectile.hpp"
#include "main.hpp"

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
	loadWeaponTextures();

	mapGenerator = new MapGenerator;
	map = new Map(mapGenerator->generateMap());
	light = new Light();
	player = new Player(*playerTexture, randomSpawn());

	app->setFramerateLimit(60);
	initializeLighting();
	initializeView();
	initializeWeapons();
	spawnEnemies(5);
	spawnWeapons(20);
}

Game::~Game()
{
	delete map;
	delete mapGenerator;
	delete light;
	delete bulletTexture;
	delete laserBeamTexture;
	delete pelletTexture;
	delete pistolTexture;
	delete shotgunTexture;
	delete playerTexture;
	delete enemyMeleeTexture;
	delete cursorTexture;
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
		updateWeapons();
		checkProjectileCollisions();
		checkEnemyProjectileCollisions();

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

	drawProjectiles();
	drawEnemies();
	drawWeapons();
	drawPlayer();
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
			heldWeapon = (heldWeapon + 1) % playerWeapons.size();
		} else if (event.key.code == sf::Keyboard::Q) {
			if (heldWeapon == 0) {
				heldWeapon = playerWeapons.size();
			}
			heldWeapon = (heldWeapon - 1);
		} else if (event.key.code == sf::Keyboard::G) {
			dropWeapon();
			pickWeapon();
		}
		break;
	}
	case sf::Event::MouseButtonPressed: {
		if (event.mouseButton.button == sf::Mouse::Left)
			shoot();
		if (event.mouseButton.button == sf::Mouse::Right)
			spawnEnemies(1);
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

sf::Vector2f Game::randomSpawn()
{
	int coll = 1;
	sf::Vector2f coords;
	float randX, randY;
	while (coll == 1) {
		randX = rand() % MAP_SIZE_X * 32;
		randY = rand() % MAP_SIZE_Y * 32;
		coll = map->collision(randX, randY, "asd");
	}
	coords = sf::Vector2f(randX, randY);
	return coords;
}

void Game::loadCharacterTextures()
{
	playerTexture = new sf::Texture();
	playerTexture->loadFromFile("media/ddos-dude-guns.png");

	enemyMeleeTexture = new sf::Texture();
	enemyMeleeTexture->loadFromFile("media/ddos-dude-guns.png");
}

void Game::shoot()
{
	if (playerWeapons[heldWeapon].ammoType == 0) {
		projectiles.push_back(BulletSprite(*bulletTexture, player->sprite.getPosition(),
		                                    sf::Vector2i(app->mapPixelToCoords(sf::Mouse::getPosition(*app))),
		                                    playerWeapons[heldWeapon].spreadAngle));
	} else if (playerWeapons[heldWeapon].ammoType == 1) {
		projectiles.push_back(LaserSprite(*laserBeamTexture, player->sprite.getPosition(),
		                                    sf::Vector2i(app->mapPixelToCoords(sf::Mouse::getPosition(*app))),
		                                    playerWeapons[heldWeapon].spreadAngle));
	} else if (playerWeapons[heldWeapon].ammoType == 2) {
		for (int i = 0; i < playerWeapons[heldWeapon].bullets; i++) {
			projectiles.push_back(PelletSprite(*pelletTexture, player->sprite.getPosition(),
			                                sf::Vector2i(app->mapPixelToCoords(sf::Mouse::getPosition(*app))),
			                                playerWeapons[heldWeapon].spreadAngle));
		}
	}
	else {
		std::cout << "heldWeapon fail: " << heldWeapon << std::endl;
	}
}

void Game::loadCursorTexture()
{
	app->setMouseCursorVisible(false);
	fixed = app->getView();

	cursorTexture = new sf::Texture();
	cursorTexture->loadFromFile("media/cursor.png");
	cursorTexture->setSmooth(true);

	cursorSprite = new sf::Sprite(*cursorTexture);
	sf::Vector2u spriteSize = cursorTexture->getSize();
	cursorSprite->setOrigin(spriteSize.x / 2, spriteSize.y / 2);
	cursorSprite->setColor(sf::Color(255, 0, 0, 255));
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

	pelletTexture = new sf::Texture();
	pelletTexture->loadFromFile("media/pellet.png");
	pelletTexture->setSmooth(true);
}

void Game::updateProjectiles()
{
	for (unsigned int i = 0; i < projectiles.size();) {
		projectiles[i].update(frameClock);
		i++;
	}

	for (unsigned int b = 0; b < enemyProjectiles.size();) {
		enemyProjectiles[b].update(frameClock);
		b++;
	}
}

void Game::checkProjectileCollisions()
{
	for (unsigned int i = 0; i < projectiles.size(); i++) {
		int x, y;
		x = projectiles[i].position.x;
		y = projectiles[i].position.y;
		if (checkEnemyCollisions(x, y, projectiles[i].getDamage()) == 1) {
			projectiles.erase(projectiles.begin() + i);
			break;
		}
		if (collision(x, y, "projectile") == 1) {
			projectiles.erase(projectiles.begin() + i);
			break;
		}
	}
}

int Game::checkEnemyCollisions(int x, int y, int damage)
{
	int enemyX, enemyY, diffX, diffY;
	int enemyCollision = 0;

	for (unsigned int b = 0; b < enemies.size(); b++) {
		enemyX = enemies[b].sprite.getPosition().x;
		enemyY = enemies[b].sprite.getPosition().y;

		diffX = abs(x - enemyX);
		diffY = abs(y - enemyY);
		if (diffX < 10 && diffY < 10) {
			enemyCollision = 1;
			enemies[b].setDamage(damage);
			enemies[b].setAggro(5);
		}
	}
	return enemyCollision;
}

void Game::checkEnemyProjectileCollisions()
{
	for (unsigned int a = 0; a < enemyProjectiles.size(); a++) {
		int x, y;
		x = enemyProjectiles[a].position.x;
		y = enemyProjectiles[a].position.y;

		if (checkPlayerCollisions(x, y, enemyProjectiles[a].getDamage()) == 1) {
			enemyProjectiles.erase(enemyProjectiles.begin() + a);
			break;
		}

		if (collision(x, y, "projectile") == 1) {
			enemyProjectiles.erase(enemyProjectiles.begin() + a);
			break;
		}
	}
}

int Game::checkPlayerCollisions(int x, int y, int damage)
{
	sf::Vector2f playerCoords(player->sprite.getPosition());
	int diffX, diffY;
	int playerCollision = 0;

	diffX = abs(x - playerCoords.x);
	diffY = abs(y - playerCoords.y);
	if (diffX < 10 && diffY < 10) {
		playerCollision = 1;
		player->setDamage(damage);
	}
	return playerCollision;
}

void Game::drawProjectiles()
{
	for (ProjectileSprite &projectile : projectiles) {
		app->draw(projectile.sprite);
	}
	for (ProjectileSprite &projectile : enemyProjectiles) {
		app->draw(projectile.sprite);
	}
}

void Game::loadWeaponTextures()
{
	pistolTexture = new sf::Texture();
	pistolTexture->loadFromFile("media/ddos-dude-guns.png");

	laserRifleTexture = new sf::Texture();
	laserRifleTexture->loadFromFile("media/ddos-dude-guns.png");

	shotgunTexture = new sf::Texture();
	shotgunTexture->loadFromFile("media/ddos-dude-guns.png");
}

void Game::spawnWeapons(int amount) {
	for (int i = 0; i < amount; i++) {
		int tmp = rand() % weapons.size();
		if (tmp == 0) {
			weaponsOnMap.push_back(Pistol(*pistolTexture));
		}
		else if (tmp == 1) {
			weaponsOnMap.push_back(LaserRifle(*laserRifleTexture));
		}
		else {
			weaponsOnMap.push_back(Shotgun(*shotgunTexture));
		}
		weaponsOnMap[i].sprite.setPosition(randomSpawn());
		weaponsOnMap[i].sprite.setRotation(rand() % 360);
	}
}

void Game::spawnWeapons(int heldWeapon, int x, int y) {
		weaponsOnMap.push_back(weapons[playerWeapons[heldWeapon].weaponPosition]);
		weaponsOnMap.back().sprite.setPosition(x, y);
		weaponsOnMap.back().sprite.setRotation(rand() % 360);
}

void Game::updateWeapons()
{
	playerWeapons[heldWeapon].update(player->sprite.getPosition().x,
	                                 player->sprite.getPosition().y,
	                                 mouse.x, mouse.y);
}

void Game::drawWeapons()
{
	app->draw(playerWeapons[heldWeapon].sprite);
	for (int i = 0; i < weaponsOnMap.size(); i++) {
		app->draw(weaponsOnMap[i].sprite);
	}
}

void Game::initializeWeapons()
{
	weapons.push_back(Pistol(*pistolTexture));
	weapons.push_back(LaserRifle(*laserRifleTexture));
	weapons.push_back(Shotgun(*shotgunTexture));

	playerWeapons.push_back(Pistol(*pistolTexture));
	playerWeapons.push_back(LaserRifle(*laserRifleTexture));
}

void Game::dropWeapon() {
	if (playerWeapons.size() > 1) {
		spawnWeapons(heldWeapon, player->sprite.getPosition().x, player->sprite.getPosition().y);
		playerWeapons.erase(playerWeapons.begin() + heldWeapon);
		heldWeapon = 0;
	}
}

void Game::pickWeapon() {
	for (int i = 0; i < weaponsOnMap.size(); i++) {
		if (playerWeapons.size() < 2) {
			sf::Vector2f playerCoords(player->sprite.getPosition());
			int diffX, diffY;
			int playerCollision = 0;

			diffX = abs(weaponsOnMap[i].sprite.getPosition().x - playerCoords.x);
			diffY = abs(weaponsOnMap[i].sprite.getPosition().y - playerCoords.y);
			if (diffX < 10 && diffY < 10) {
				playerWeapons.push_back(weapons[weaponsOnMap[i].weaponPosition]);
				weaponsOnMap.erase(weaponsOnMap.begin() + i);
				heldWeapon = 1;
			}
		}
	}
}

void Game::spawnEnemies(int amount)
{
	for (int i = 0; i < amount; i++) {
		enemies.push_back(EnemyMelee(*enemyMeleeTexture, randomSpawn()));
		std::cout << "Enemy Spawned" << std::endl;
	}
}

void Game::updateEnemies()
{
	for (unsigned int i = 0; i <  enemies.size();) {
		if (enemies[i].getHitpoints() <= 0) {
			enemies.erase(enemies.begin() + i);
			break;
		}

		enemies[i].update(enemies[i].sprite.getPosition().x,
		                  enemies[i].sprite.getPosition().y,
		                  player->sprite.getPosition().x,
		                  player->sprite.getPosition().y);
		if (checkProximity(enemies[i].sprite.getPosition()) == 1) {
			player->setDamage(enemies[i].getMeleeDamage());
			std::cout << "Hitpoints: " << player->getHitpoints() << std::endl;
		}
		i++;
	}
}

int Game::checkProximity(sf::Vector2f enemy)
{
	sf::Vector2f playerPosition(player->sprite.getPosition());
	float distanceX, distanceY, distanceFromObject;
	int closeEnough = 0;
	distanceX = abs(enemy.x - playerPosition.x);
	distanceY = abs(enemy.y - playerPosition.y);
	//pitää olla sama kuin objectin approachissa, muutetaan myöhemmin
	distanceFromObject = sqrt(distanceX * 2 + distanceY * 2);

	if (distanceFromObject < 5) {
		closeEnough = 1;
	}
	return closeEnough;
}

void Game::drawEnemies()
{
	for (unsigned int i = 0; i < enemies.size(); i++) {
		app->draw(enemies[i].sprite);
	}
}

void Game::drawPlayer()
{
	app->draw(player->sprite);
}

void Game::drawCursor()
{
	mouse = sf::Vector2i(app->mapPixelToCoords(sf::Mouse::getPosition(*app)));
	cursorSprite->setPosition(static_cast<sf::Vector2f>(mouse));
	app->draw(*cursorSprite);
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
