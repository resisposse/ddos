/*
 * Dark Domains Of Space
 * 2015 © Project Team (see: LICENSE)
 */

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include "globals.hpp"
#include "audio.hpp"
#include "light.hpp"
#include "map.hpp"
#include "mapgenerator.hpp"
#include "object.hpp"
#include "projectile.hpp"
#include "weapon.hpp"
#include "mainmenu.hpp"
#include "gamestate.hpp"
#include "gameover.hpp"
#include "pause.hpp"
#include "random.hpp"
#include "node.hpp"
#include "grid.hpp"
#include "pathfinding.hpp"
#include "main.hpp"

float frameClock = 0;
long lastClock = 0;
sf::Clock timer;
Game *game;

Game::Game(StateManager *stateManager)
{
	this->stateManager = stateManager;
	lastClock = timer.getElapsedTime().asMilliseconds();

	loadTextures();

	random = new Random;
	mapGenerator = new MapGenerator;
	audio = new Audio;
	light = new Light;
	lightState = new LightState;
	player = new Player(*playerTexture, sf::Vector2f(0,0));
	healthbar = new HealthBar(*healthTexture);
	shieldbar = new ShieldBar(*shieldTexture);

	initializeView();
	initializeLighting();
	initializeWeapons();
	initializeHUD();
	createNewStage();
}

Game::~Game()
{
	delete map;
	delete audio;
	delete light;
	delete bulletTexture;
	delete laserBeamTexture;
	delete pelletTexture;
	delete weaponTexture;
	delete playerTexture;
	delete enemyMeleeTexture;
	delete cursorTexture;
	delete healthTexture;
	delete valuableTexture;
	delete player;
	delete playerView;
	delete healthbar;
	delete shieldTexture;
	delete shieldbar;
	delete app;
	delete mapGenerator;
	delete blood8x8Texture;
	delete blood16x16Texture;
	clearVectors();
}

void Game::draw()
{
	/*
	* Not to be confused with initializeLighting(), which just initializes
	* values for the light to use. This on the other hand paints the screen
	* black on every frame, so we can update the position of the light.
	*/
	light->initialize();

	map->renderTiles();

	drawGore();
	drawMapValuables();
	drawMapWeapons();
	drawEnemies();
	drawProjectiles();
	drawPlayer();
	drawWeapon();

	light->update(&lightState->tmpSource);

	drawCursor();
	drawHealthbar();
	drawHUDText();
	drawShieldBar();
	drawCurrentGun();
	drawCurrentAmmo();
	HUDManager();
}

void Game::update()
{
	updateClock();
	updateView();
	updateLighting();
	updatePlayer();
	updateEnemies();
	updateWeapons();
	updateProjectiles();
	checkProjectileCollisions();
	checkEnemyProjectileCollisions();
	pickValuables();
	checkPlayerDeath();
}

void Game::handleInput()
{
	sf::Event event;
	tempClock += frameClock;
	playFootstepSound();
	while (app->pollEvent(event)) {
		switch (event.type) {
		case sf::Event::KeyPressed:
			switch (event.key.code) {
			case sf::Keyboard::W:
				player->mIsMovingUp = true;
				break;
			case sf::Keyboard::A:
				player->mIsMovingLeft = true;
				break;
			case sf::Keyboard::S:
				player->mIsMovingDown = true;
				break;
			case sf::Keyboard::D:
				player->mIsMovingRight = true;
				break;
			case sf::Keyboard::E:
				heldWeapon = (heldWeapon + 1) % playerWeapons.size();
				break;
			case sf::Keyboard::Q:
				if (heldWeapon == 0) {
					heldWeapon = playerWeapons.size();
				}
				heldWeapon = (heldWeapon - 1);
				break;
			case sf::Keyboard::G:
				dropWeapon();
				pickWeapon();
				break;
			case sf::Keyboard::T: {
				int i = (int)playerPositionX / 32;
				int j = (int)playerPositionY / 32;
				if (map->tiles[i][j].type == MapTileType::mtGoal) {
					audio->teleportSound->play();
					createNewStage();
				}
				break;
			}
			case sf::Keyboard::Escape:
				app->setMouseCursorVisible(true);
				stateManager->pushState(new Pause(stateManager));
				break;
			default:
				break;
			}
			break;
		case sf::Event::KeyReleased:
			switch (event.key.code) {
			case sf::Keyboard::W:
				player->mIsMovingUp = false;
				break;
			case sf::Keyboard::A:
				player->mIsMovingLeft = false;
				break;
			case sf::Keyboard::S:
				player->mIsMovingDown = false;
				break;
			case sf::Keyboard::D:
				player->mIsMovingRight = false;
				break;
			default:
				break;
			}
			break;
		case sf::Event::MouseButtonPressed:
			switch (event.mouseButton.button) {
			case sf::Mouse::Left:
				player->playerShooting = true;
				break;
			case sf::Mouse::Right:
				spawnEnemies(1);
				break;
			default:
				break;
			}
			break;
		case sf::Event::MouseButtonReleased:
			switch (event.mouseButton.button) {
			case sf::Mouse::Left:
				player->playerShooting = false;
				break;
			case sf::Mouse::Right:
				light->clear();
				break;
			default:
				break;
			}
			break;
		case sf::Event::MouseWheelMoved:
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
		case sf::Event::Closed:
			app->close();
			break;
		case sf::Event::Resized: {
			playerView->setSize(event.size.width, event.size.height);
			map->bgSpr->setPosition(app->mapPixelToCoords(sf::Vector2i(0, 0),
				*playerView));
			sf::Vector2f pos = sf::Vector2f(event.size.width, event.size.height);
			pos *= 0.5f;
			pos = app->mapPixelToCoords(sf::Vector2i(pos), *playerView);
			map->bgSpr->setScale(float(event.size.width) / float(map->bgSpr->getTexture()->getSize().x),
				float(event.size.height) / float(map->bgSpr->getTexture()->getSize().y));
			break;
		}
		default:
			break;
		}
	}
}

void Game::playFootstepSound()
{
	float playerPositionX = game->player->sprite.getPosition().x;
	float playerPositionY = game->player->sprite.getPosition().y;
	int randomValue;
	if (player->mIsMovingUp == true ||
	    player->mIsMovingLeft == true ||
	    player->mIsMovingDown == true ||
	    player->mIsMovingRight == true) {
		if (isPlaying == false) {
			tempClock = 0.0;
			randomValue = random->generate(0,audio->footsteps.size()-1);
			audio->footsteps[randomValue]->setVolume(MISC_VOLUME);
			audio->footsteps[randomValue]->setPosition(0,0,0);
			audio->footsteps[randomValue]->play();
			isPlaying = true;
		} else if (isPlaying == true) {
			if (tempClock > 0.35) {
				isPlaying = false;
			}
		}
	}

}

void Game::loadTextures()
{
	app->setMouseCursorVisible(false);
	cursorTexture = new sf::Texture();
	cursorTexture->loadFromFile("media/cursor.png");
	cursorTexture->setSmooth(true);
	cursorSprite = new sf::Sprite(*cursorTexture);
	sf::Vector2u spriteSize = cursorTexture->getSize();
	cursorSprite->setOrigin(spriteSize.x / 2, spriteSize.y / 2);
	cursorSprite->setColor(sf::Color(255, 0, 0, 255));

	healthTexture = new sf::Texture();
	healthTexture->loadFromFile("media/laserBeam.png");
	healthTexture->setSmooth(true);
	shieldTexture = new sf::Texture();
	shieldTexture->loadFromFile("media/shieldBar.png");
	shieldTexture->setSmooth(true);

	playerTexture = new sf::Texture();
	playerTexture->loadFromFile("media/ddos-dude-guns.png");
	enemyMeleeTexture = new sf::Texture();
	enemyMeleeTexture->loadFromFile("media/ddos-sprites.png");

	weaponTexture = new sf::Texture();
	weaponTexture->loadFromFile("media/ddos-weapons.png");

	bulletTexture = new sf::Texture();
	bulletTexture->loadFromFile("media/bullet.png");
	bulletTexture->setSmooth(true);
	laserBeamTexture = new sf::Texture();
	laserBeamTexture->loadFromFile("media/laserBeam.png");
	laserBeamTexture->setSmooth(true);
	pelletTexture = new sf::Texture();
	pelletTexture->loadFromFile("media/pellet.png");
	pelletTexture->setSmooth(true);
	heavyBulletTexture = new sf::Texture();
	heavyBulletTexture->loadFromFile("media/heavyBullet.png");
	heavyBulletTexture->setSmooth(true);

	/* We ought to not lose this texture, it's very valuable */
	valuableTexture = new sf::Texture();
	valuableTexture->loadFromFile("media/coin.png");

	blood8x8Texture = new sf::Texture();
	blood8x8Texture->loadFromFile("media/blood8x8.png");
	blood16x16Texture = new sf::Texture();
	blood16x16Texture->loadFromFile("media/blood16x16.png");
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
 * These values are used for every light source as long as they are not
 * overridden elsewhere (or more often, hardcoded into their
 * implementation).
 */
void Game::initializeLighting()
{
	lightState->brush.intensity = 100;
	lightState->brush.color = sf::Color::White;
	lightState->brush.color.r = 255;
	lightState->brush.color.g = 255;
	lightState->brush.color.b = 255;
	lightState->brush.sourceTime = 10.0f;
}

void Game::initializeWeapons()
{
	weapons.push_back(new Pistol(*weaponTexture));
	weapons.push_back(new LaserRifle(*weaponTexture));
	weapons.push_back(new Shotgun(*weaponTexture));
	weapons.push_back(new MachineGun(*weaponTexture));
	weapons.push_back(new SniperRifle(*weaponTexture));

	playerWeapons.push_back(new Pistol(*weaponTexture));
	playerWeapons.push_back(new LaserRifle(*weaponTexture));
}

void Game::initializeHUD()
{
	if (!font.loadFromFile("fonts/Exo-Regular.otf")) {
		//error
	}
	healthText.setCharacterSize(15);
	scoreText.setCharacterSize(15);
	currentGun.setCharacterSize(15);
	currentAmmo.setCharacterSize(15);
}

void Game::spawnEnemies(int amount)
{
	for (int i = 0; i < amount/2; i++) {
		enemies.push_back(new EnemyMelee(*enemyMeleeTexture, randomSpawn()));
		//std::cout << "Enemy Melee Spawned" << i << std::endl;
	}
	for (int i = amount/2;  i < amount; i++) {
		enemies.push_back(new EnemySoldier(*enemyMeleeTexture, randomSpawn()));
		//std::cout << "Enemy Soldier Spawned" << i << std::endl;
	}
}

void Game::spawnWeapons(int amount)
{
	for (int i = 0; i < amount; i++) {
		int tmp = rand() % weapons.size();
		if (tmp == 0) {
			mapWeapons.push_back(new Pistol(*weaponTexture));
		} else if (tmp == 1) {
			mapWeapons.push_back(new LaserRifle(*weaponTexture));
		} else if (tmp == 2) {
			mapWeapons.push_back(new Shotgun(*weaponTexture));
		} else if (tmp == 3) {
			mapWeapons.push_back(new MachineGun(*weaponTexture));
		} else {
			mapWeapons.push_back(new SniperRifle(*weaponTexture));
		}
		mapWeapons[i]->sprite.setPosition(randomSpawn());
		mapWeapons[i]->sprite.setRotation(rand() % 360);
		mapWeapons[i]->sprite.setOrigin(16,8);
	}
}

void Game::spawnValuables(int totalValue)
{
	for (int i = 0; i < totalValue; i += mapValuables.back()->getValue()) {
		int r = rand() % 3 + 1;
		if (r == 1) {
			mapValuables.push_back(new ValuableLow(*valuableTexture, randomSpawn()));
		} else if (r == 2) {
			mapValuables.push_back(new ValuableMed(*valuableTexture, randomSpawn()));
		} else {
			mapValuables.push_back(new ValuableHigh(*valuableTexture, randomSpawn()));
		}
	}
}

void Game::updateClock()
{
	currentClock += timer.getElapsedTime().asMilliseconds();
	frameClock = (currentClock - lastClock) / 1000.0;
	if (frameClock > 0.1) {
		frameClock = 0.1;
	}
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
		if (enemies[i]->getHitpoints() <= 0) {
			score += enemies[i]->getValue();
			for (int tmp = 0; tmp < 3 + rand() % 3; tmp++) {
				mapBlood.push_back(new BloodMedium(*blood16x16Texture, enemies[i]->sprite.getPosition()));
			}
			enemies.erase(enemies.begin() + i);
			break;
		}
		enemies[i]->update(enemies[i]->sprite.getPosition().x,
		                   enemies[i]->sprite.getPosition().y,
		                   player->sprite.getPosition().x,
		                   player->sprite.getPosition().y);
		if (checkProximity(enemies[i]->sprite.getPosition()) == 1) {
			player->setDamage(enemies[i]->getMeleeDamage());
		}
		i++;
	}
}

void Game::updateWeapons()
{
	playerWeapons[heldWeapon]->update(player->sprite.getPosition().x,
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

		if (map->collision2(x, y, "projectile") == 1) {
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
		enemyX = enemies[b]->sprite.getPosition().x;
		enemyY = enemies[b]->sprite.getPosition().y;

		diffX = abs(x - enemyX);
		diffY = abs(y - enemyY);
		if (diffX < 10 && diffY < 10) {
			for (int tmp = 0; tmp < 1 + rand() % 3; tmp++) {
				mapBlood.push_back(new BloodSmall(*blood8x8Texture, enemies[b]->sprite.getPosition()));
			}
			enemyCollision = 1;
			enemies[b]->setDamage(damage);
			enemies[b]->setAggro(5);
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
		player->shieldTimeUntilRecharge = 0.0;
	}
	return playerCollision;
}

void Game::drawEnemies()
{
	for (unsigned int i = 0; i < enemies.size(); i++) {
		app->draw(enemies[i]->sprite);
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
	app->draw(playerWeapons[heldWeapon]->sprite);
}

void Game::drawMapWeapons()
{
	for (unsigned int i = 0; i < mapWeapons.size(); i++) {
		app->draw(mapWeapons[i]->sprite);
	}
}

void Game::drawMapValuables()
{
	for (unsigned int i = 0; i < mapValuables.size(); i++) {
		app->draw(mapValuables[i]->sprite);
	}
}

void Game::drawHealthbar()
{
	int hbarLength = player->getHitpoints();
	sf::IntRect mCurrentHealth(0, 0, hbarLength, 5);
	healthbar->sprite.setTextureRect(mCurrentHealth);
	app->draw(healthbar->sprite);
}

void Game::drawShieldBar()
{
	int sbarLength = player->getShieldpoints();
	sf::IntRect mCurrentShield(0, 0, sbarLength, 5);
	shieldbar->sprite.setTextureRect(mCurrentShield);
	app->draw(shieldbar->sprite);
}

void Game::drawHUDText()
{
	app->draw(healthText);
	app->draw(scoreText);
}

void Game::drawCurrentGun()
{
	app->draw(currentGun);
}

void Game::drawCurrentAmmo()
{
	app->draw(currentAmmo);
}

void Game::drawGore()
{
	int bloodSprsOnMap = 2500;
	while (mapBlood.size() > bloodSprsOnMap) {
		mapBlood.erase(mapBlood.begin());
	}
	for (unsigned int i = 0; i < mapBlood.size(); i++) {
		app->draw(mapBlood[i]->sprite);
	}
}

void Game::drawCursor()
{
	mouse = sf::Vector2i(app->mapPixelToCoords(sf::Mouse::getPosition(*app)));
	cursorSprite->setPosition(static_cast<sf::Vector2f>(mouse));
	app->draw(*cursorSprite);

}

void Game::dropWeapon()
{
	if (playerWeapons.size() > 1) {
		int playerPositionX = player->sprite.getPosition().x;
		int playerPositionY = player->sprite.getPosition().y;
		mapWeapons.push_back(playerWeapons[heldWeapon]);
		mapWeapons.back()->sprite.setPosition(playerPositionX, playerPositionY);
		mapWeapons.back()->sprite.setRotation(rand() % 360);
		mapWeapons.back()->sprite.setOrigin(16, 8);
		playerWeapons.erase(playerWeapons.begin() + heldWeapon);
		heldWeapon = 0;
	}
}

void Game::pickWeapon()
{
	for (unsigned int i = 0; i < mapWeapons.size(); i++) {
		if (playerWeapons.size() < 2) {
			sf::Vector2f playerCoords(player->sprite.getPosition());
			int diffX, diffY;

			diffX = abs(mapWeapons[i]->sprite.getPosition().x - playerCoords.x);
			diffY = abs(mapWeapons[i]->sprite.getPosition().y - playerCoords.y);
			if (diffX < 10 && diffY < 10) {
				playerWeapons.push_back(mapWeapons[i]);
				playerWeapons.back()->sprite.setOrigin(16, 0);
				mapWeapons.erase(mapWeapons.begin() + i);
				heldWeapon = 1;

				audio->dropgunSound->play();
			}
		}
	}
}

void Game::pickValuables()
{
	for (unsigned int i = 0; i < mapValuables.size(); i++) {
		sf::Vector2f playerCoords(player->sprite.getPosition());
		int diffX, diffY;

		diffX = abs(mapValuables[i]->sprite.getPosition().x - playerCoords.x);
		diffY = abs(mapValuables[i]->sprite.getPosition().y - playerCoords.y);
		if (diffX < 20 && diffY < 20) {
			score += mapValuables[i]->getValue();
			mapValuables.erase(mapValuables.begin() + i);

			audio->coinSound->play();
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

	currentGun.setString(playerWeapons[heldWeapon]->name);
	currentGun.setFont(font);
	currentGun.setPosition(wWGun/2 + weaponHUDX , wHGun/2 + weaponHUDY);

	/*window SE-corner*/
	float wWAmmo = (playerView->getSize().x);
	float wHAmmo = (playerView->getSize().y);
	ammoHUDX = playerPositionX - 155;
	ammoHUDY = playerPositionY - 30;

	currentAmmo.setString("Ammo: " + std::to_string (playerWeapons[heldWeapon]->getAmmo()));
	currentAmmo.setFont(font);
	currentAmmo.setPosition(wWAmmo / 2 + ammoHUDX, wHAmmo / 2 + ammoHUDY);

	/*window SW-corner*/
	float wW = (playerView->getSize().x) * (-1);
	float wH = (playerView->getSize().y);
	healthbarPositionX = playerPositionX + 10;
	healthbarPositionY = playerPositionY - 15;
	healthTextPositionX = playerPositionX + 10;
	healthTextPositionY = playerPositionY - 50;

	healthText.setString("Health: " + std::to_string (player->getHitpoints()));
	healthText.setFont(font);
	healthText.setPosition(wW / 2 + healthTextPositionX, wH / 2 + healthTextPositionY);
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

	/* window middle */
	float wWShield = (playerView->getSize().x) * (-1);
	float wHShield = (playerView->getSize().y);
	shieldbarPositionX = playerPositionX + 115;
	shieldbarPositionY = playerPositionY - 15;

	shieldbar->sprite.setPosition(wWShield / 2 + shieldbarPositionX, wHShield / 2 + shieldbarPositionY);
	shieldbar->sprite.setColor(sf::Color(0, 100, 255));

	scoreTextPositionX = playerPositionX + 10;
	scoreTextPositionY = playerPositionY - 35;

	scoreText.setString("Score: " + std::to_string(score));
	scoreText.setFont(font);
	scoreText.setPosition(wW / 2 + scoreTextPositionX, wH / 2 + scoreTextPositionY);
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
	int collision = 1;
	float randX, randY;
	sf::Vector2f coords;
	sf::Vector2f playerPosition;
	playerPositionX = player->sprite.getPosition().x;
	playerPositionY = player->sprite.getPosition().y;
	/*
	 * A loop for generating spawn coordinates and checking whether they are
	 * 	1. Colliding with the wall
	 * 	2. Too close to the player's spawn room
	 * 	3. In the goal room
	 * If so, we make sure the loop is ran again by overriding whatever
	 * value is returned by map->collision().
	 */
	while (collision == 1) {
		randX = rand() % MAP_SIZE_X * 32;
		randY = rand() % MAP_SIZE_Y * 32;
		collision = map->collision2(randX, randY, "asd");
		if ((abs(randX - playerPositionX) < (8 * TILE_SIZE) &&
		     abs(randY - playerPositionY) < (8 * TILE_SIZE)) ||
		     (map->tiles[(int)randX / 32][(int)randY / 32].type == mtGoal ||
		      map->tiles[(int)randX / 32][(int)randY / 32].type == mtDoor)) {
			collision = 1;
		}
	}
	coords = sf::Vector2f(randX, randY);
	return coords;
}

sf::Vector2f Game::playerSpawn()
{
	sf::Vector2f coords;
	float spawnX, spawnY;
	spawnX = (float)mapGenerator->spawn.x * TILE_SIZE + TILE_SIZE / 2;
	spawnY = (float)mapGenerator->spawn.y * TILE_SIZE + TILE_SIZE / 2;
	coords = sf::Vector2f(spawnX, spawnY);
	return coords;
}

void Game::clearVectors()
{
	deleteList(mapWeapons);
	deleteList(enemies);
	deleteList(mapValuables);
	deleteList(mapBlood);
}

void Game::createNewStage()
{
	Map *mapOld = map;
	map = new Map(mapGenerator->generateMap());
	/* Does not work on Linux with gcc.
	if (mapOld) {
		delete mapOld;
	}*/
	player->sprite.setPosition(playerSpawn());

	clearVectors();
	spawnEnemies(30);
	spawnWeapons(15);
	spawnValuables(1000);
	pathfindAlustus = false;
}

std::vector<sf::Vector2f> Game::getPath(sf::Vector2f enemyCoords) {
	if (!pathfindAlustus) {
		/* Does not work on Linux with gcc.
		if (pathfinding) {
			delete pathfinding;
		}*/
		pathfinding = new Pathfinding();
		pathfindAlustus = true;
	}
	std::vector<sf::Vector2f> path(pathfinding->findPath(player->sprite.getPosition(), enemyCoords));
	return path;
}

void Game::checkPlayerDeath()
{
	if (player->getHitpoints() <= 0) {
		endGame();
	}
}

void Game::endGame()
{
	app->setMouseCursorVisible(true);
	stateManager->changeState(new GameOver(stateManager));
}

void Game::returnToMainMenu()
{
	stateManager->popState();
}

int main()
{
	sf::Music music;
	music.openFromFile("audio/smilecythe_-_penetrating_breeze.ogg");
	music.setVolume(MUSIC_VOLUME);
	music.setLoop(true);
	music.play();

	StateManager stateManager;
	stateManager.pushState(new MainMenu(&stateManager));
	stateManager.gameLoop();
	return EXIT_SUCCESS;
}
