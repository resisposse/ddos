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
#include "weapon.hpp"
#include "main.hpp"

float frameClock = 0;
long lastClock = 0;
sf::Clock timer;

sf::RenderWindow *app;
Game *game;

Game::Game()
{
	app = new sf::RenderWindow(sf::VideoMode(800, 608, 32),
	                           "Dark Domains Of Space",
	                           sf::Style::Resize | sf::Style::Close);
	running = true;
	shooting = false;
	lastClock = timer.getElapsedTime().asMilliseconds();
	app->setFramerateLimit(60);

	loadCursorTexture();
	loadHealthbarTexture();
	loadCharacterTextures();
	loadWeaponTextures();
	loadProjectileTextures();

	mapGenerator = new MapGenerator;
	map = new Map(mapGenerator->generateMap());
	light = new Light();
	lightState = new LightState();
	player = new Player(*playerTexture, randomSpawn());
	healthbar = new HealthBar(*healthTexture);

	initializeView();
	initializeLighting();
	initializeWeapons();
	initializeHUD();
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
	delete healthTexture;
	delete player;
	delete playerView;
	delete healthbar;
	delete app;
}

void Game::update()
{
	while (running) {
		parseEvents();

		updateClock();
		updateView();
		updateLighting();
		updatePlayer();
		updateEnemies();
		updateWeapons();
		updateProjectiles();
		checkProjectileCollisions();
		checkEnemyProjectileCollisions();

		app->clear();
		render();
		HUDManager();
		app->display();
	}
}

void Game::render()
{
	light->initialize();
	map->renderTiles();

	drawWeaponsOnMap();
	drawEnemies();
	drawProjectiles();
	drawPlayer();
	drawWeapon();

	light->update(&lightState->tmpSource);

	drawCursor();
	drawHealthbar();
	drawHealthText();
	drawCurrentGun();
	drawCurrentAmmo();
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

void Game::loadHealthbarTexture()
{
	healthTexture = new sf::Texture();
	healthTexture->loadFromFile("media/laserBeam.png");
	healthTexture->setSmooth(true);
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

void Game::loadWeaponTextures()
{
	pistolTexture = new sf::Texture();
	pistolTexture->loadFromFile("media/ddos-dude-guns.png");
	pistolTexture->setSmooth(true);

	laserRifleTexture = new sf::Texture();
	laserRifleTexture->loadFromFile("media/ddos-dude-guns.png");
	laserRifleTexture->setSmooth(true);

	shotgunTexture = new sf::Texture();
	shotgunTexture->loadFromFile("media/ddos-dude-guns.png");
	shotgunTexture->setSmooth(true);
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

void Game::initializeLighting()
{
	lightState->brush.type = stStatic;
	lightState->brush.intensity = 100;
	lightState->brush.color = sf::Color::White;
	lightState->brush.color.r = 255;
	lightState->brush.color.g = 255;
	lightState->brush.color.b = 255;
	lightState->brush.sourceTime = 2.0f;
	lightState->ambientIntensity = 0;
	lightState->ambientColor = sf::Color::White;
	lightState->ambientColor.r = 0;
	lightState->ambientColor.g = 0;
	lightState->ambientColor.b = 0;

	/* Options are: stStatic, stPulsing, stFading, stTest */
	lightState->brush.type = stPulsing;
}

void Game::initializeWeapons()
{
	weapons.push_back(Pistol(*pistolTexture));
	weapons.push_back(LaserRifle(*laserRifleTexture));
	weapons.push_back(Shotgun(*shotgunTexture));

	playerWeapons.push_back(Pistol(*pistolTexture));
	playerWeapons.push_back(LaserRifle(*laserRifleTexture));
}

void Game::initializeHUD()
{
	if (!font.loadFromFile("fonts/arial.ttf")) {
		//error
	}
	healthText.setCharacterSize(15);
	currentGun.setCharacterSize(15);
	currentAmmo.setCharacterSize(15);
}

void Game::spawnEnemies(int amount)
{
	for (int i = 0; i < amount; i++) {
		enemies.push_back(EnemyMelee(*enemyMeleeTexture, randomSpawn()));
		std::cout << "Enemy Spawned" << std::endl;
	}
}

void Game::spawnWeapons(int amount)
{
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

void Game::spawnWeapons(int heldWeapon, int x, int y)
{
	weaponsOnMap.push_back(weapons[playerWeapons[heldWeapon].weaponPosition]);
	weaponsOnMap.back().sprite.setPosition(x, y);
	weaponsOnMap.back().sprite.setRotation(rand() % 360);
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
			shooting = true;
		if (event.mouseButton.button == sf::Mouse::Right)
			spawnEnemies(1);
		break;
	}
	case sf::Event::MouseButtonReleased: {
		if (event.mouseButton.button == sf::Mouse::Left) shooting = false;
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

void Game::updateClock()
{
	currentClock += timer.getElapsedTime().asMilliseconds();
	frameClock = (currentClock - lastClock) / 1000.0;
	lastClock = currentClock;
	timer.restart();
}

void Game::updateView()
{
	playerView->setCenter(playerPositionX, playerPositionY);
	map->bgSpr->setOrigin(400, 300);
	map->bgSpr->setPosition(playerPositionX, playerPositionY);
	app->setView(*playerView);
}

void Game::updateLighting()
{
	playerPositionX = player->sprite.getPosition().x;
	playerPositionY = player->sprite.getPosition().y;
	lightState->brush.position = sf::Vector2i((int)playerPositionX / TILE_SIZE,
	                                          (int)playerPositionY / TILE_SIZE);
	lightState->tmpSource = StaticLightSource(lightState->brush.position,
	                                          lightState->brush.color,
	                                          lightState->brush.intensity);
	light->ambientColor = lightState->ambientColor;
	light->ambientIntensity = lightState->ambientIntensity;
}

void Game::updatePlayer()
{
	player->update(frameClock);
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

void Game::updateWeapons()
{
	playerWeapons[heldWeapon].update(player->sprite.getPosition().x,
	                                 player->sprite.getPosition().y,
	                                 mouse.x, mouse.y);
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
		if (map->collision(x, y, "projectile") == 1) {
			projectiles.erase(projectiles.begin() + i);
			break;
		}
	}
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

		if (map->collision(x, y, "projectile") == 1) {
			enemyProjectiles.erase(enemyProjectiles.begin() + a);
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

void Game::drawEnemies()
{
	for (unsigned int i = 0; i < enemies.size(); i++) {
		app->draw(enemies[i].sprite);
	}
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

void Game::drawPlayer()
{
	app->draw(player->sprite);
}

void Game::drawWeapon()
{
	app->draw(playerWeapons[heldWeapon].sprite);
}

void Game::drawCurrentAmmo()
{
	app->draw(game->currentAmmo);
}

void Game::drawWeaponsOnMap()
{
	for (unsigned int i = 0; i < weaponsOnMap.size(); i++) {
		app->draw(weaponsOnMap[i].sprite);
	}
}

void Game::drawHealthbar()
{
	int hbarLength = player->getHitpoints();
	sf::IntRect mCurrentHealth(0, 0, hbarLength, 5);
	healthbar->sprite.setTextureRect(mCurrentHealth);
	app->draw(healthbar->sprite);
}

void Game::drawHealthText()
{
	app->draw(game->healthText);
}

void Game::drawCurrentGun()
{
	app->draw(game->currentGun);
}

void Game::drawCursor()
{
	mouse = sf::Vector2i(app->mapPixelToCoords(sf::Mouse::getPosition(*app)));
	cursorSprite->setPosition(static_cast<sf::Vector2f>(mouse));
	app->draw(*cursorSprite);

}

void Game::addSource()
{
	switch (lightState->brush.type) {
	case stStatic:
		light->sources.push_back((StaticLightSource *)
		                         (new StaticLightSource
		                         (lightState->brush.position,
		                          lightState->brush.color,
		                          lightState->brush.intensity)));
		break;
	case stFading:
		light->sources.push_back((StaticLightSource *)
		                         (new FadingLightSource
		                         (lightState->brush.position,
		                          lightState->brush.color,
		                          lightState->brush.intensity,
		                          lightState->brush.sourceTime)));
		break;
	case stPulsing:
		light->sources.push_back((StaticLightSource *)
		                         (new PulsingLightSource
		                         (lightState->brush.position,
		                          lightState->brush.color,
		                          lightState->brush.intensity,
		                          lightState->brush.sourceTime)));
		break;
	case stTest:
		light->sources.push_back((StaticLightSource *)
		                         (new TestLightSource
		                         (lightState->brush.position,
		                          lightState->brush.color,
		                          lightState->brush.intensity,
		                          lightState->brush.sourceTime)));
		break;
	}
}

void Game::shoot()
{
	if (shooting == 1 && shootingCooldown <= 0) {
		shootingCooldown = weapons[heldWeapon].attackSpeed;
			ammos = weapons[heldWeapon].getAmmo();
			weapons[playerWeapons[heldWeapon].weaponPosition].setAmmo(1);
		switch (playerWeapons[heldWeapon].ammoType) {
		case 0:
			projectiles.push_back(BulletSprite(*bulletTexture, player->sprite.getPosition(),
			                                   sf::Vector2i(app->mapPixelToCoords(sf::Mouse::getPosition(*app))),
			                                   playerWeapons[heldWeapon].spreadAngle));
			break;
		case 1:
			projectiles.push_back(LaserSprite(*laserBeamTexture, player->sprite.getPosition(),
			                                  sf::Vector2i(app->mapPixelToCoords(sf::Mouse::getPosition(*app))),
			                                  playerWeapons[heldWeapon].spreadAngle));
			break;
		case 2:
			for (int i = 0; i < playerWeapons[heldWeapon].bullets; i++) {
				projectiles.push_back(PelletSprite(*pelletTexture, player->sprite.getPosition(),
				                                   sf::Vector2i(app->mapPixelToCoords(sf::Mouse::getPosition(*app))),
				                                   playerWeapons[heldWeapon].spreadAngle));
			}
			break;
		default:
			std::cout << "heldWeapon fail: " << heldWeapon << std::endl;
			break;
		}
	} else {
		shootingCooldown -= frameClock;
	}
}

void Game::dropWeapon()
{
	if (playerWeapons.size() > 1) {
		spawnWeapons(heldWeapon, player->sprite.getPosition().x, player->sprite.getPosition().y);
		playerWeapons.erase(playerWeapons.begin() + heldWeapon);
		heldWeapon = 0;
	}
}

void Game::pickWeapon()
{
	for (unsigned int i = 0; i < weaponsOnMap.size(); i++) {
		if (playerWeapons.size() < 2) {
			sf::Vector2f playerCoords(player->sprite.getPosition());
			int diffX, diffY;

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

void Game::HUDManager()
{
	/*window SE-corner*/
	float wWGun = (playerView->getSize().x);
	float wHGun = (playerView->getSize().y);
	weaponHUDX = playerPositionX - 155;
	weaponHUDY = playerPositionY - 55;

	currentGun.setString(weapons[playerWeapons[heldWeapon].weaponPosition].name);
	currentGun.setFont(font);
	game->currentGun.setPosition(wWGun/2 + weaponHUDX , wHGun/2 + weaponHUDY);

	/*window SE-corner*/
	float wWAmmo = (playerView->getSize().x);
	float wHAmmo = (playerView->getSize().y);
	ammoHUDX = playerPositionX - 155;
	ammoHUDY = playerPositionY - 20;

	currentAmmo.setString("Ammo: " + std::to_string (weapons[playerWeapons[heldWeapon].weaponPosition].getAmmo()));
	currentAmmo.setFont(font);
	game->currentAmmo.setPosition(wWAmmo / 2 + ammoHUDX, wHAmmo / 2 + ammoHUDY);

	/*window SW-corner*/
	float wW = (playerView->getSize().x) * (-1);
	float wH = (playerView->getSize().y);
	healthbarPositionX = playerPositionX + 10;
	healthbarPositionY = playerPositionY - 15;
	healthTextPositionX = playerPositionX + 10;
	healthTextPositionY = playerPositionY - 50;

	healthText.setString("Health: " + std::to_string (player->getHitpoints()));
	healthText.setFont(font);
	game->healthText.setPosition(wW / 2 + healthTextPositionX, wH / 2 + healthTextPositionY);
	healthbar->sprite.setPosition(wW / 2 + healthbarPositionX, wH / 2 + healthbarPositionY);

	if (player->getHitpoints() >= 70) {
		healthText.setColor(sf::Color::Green);
	} else if (player->getHitpoints() >= 30 && player->getHitpoints() < 70) {
		healthText.setColor(sf::Color::Yellow);
	} else {
		healthText.setColor(sf::Color::Red);
	}

	if (player->getHitpoints() >= 70) {
		healthbar->sprite.setColor(sf::Color::Green);
	} else if (player->getHitpoints() >= 30 && player->getHitpoints() < 70){
		healthbar->sprite.setColor(sf::Color::Yellow);
	} else {
		healthbar->sprite.setColor(sf::Color::Red);
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
