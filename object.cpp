/*
 * Dark Domains Of Space
 * 2015 © Project Team (see: LICENSE)
 */

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <cmath>
#include "map.hpp"
#include "audio.hpp"
#include "projectile.hpp"
#include "node.hpp"
#include "grid.hpp"
#include "pathfinding.hpp"
#include "main.hpp"
#include "weapon.hpp"
#include "object.hpp"

Object::Object(sf::Texture& objectTexture)
{
	mIsMovingUp = false;
	mIsMovingDown = false;
	mIsMovingRight = false;
	mIsMovingLeft = false;
	playerSpeed = 100.0;
	playerShooting = false;

	//sf::IntRect mPlayer(32 * 0, 32 * 0, 32, 32);
	sprite.setTexture(objectTexture);
	//sprite.setTextureRect(mPlayer);
	//sprite.setOrigin(16, 16);
}

void Object::update(float frameClock)
{
	float angle;
	double a, b;
	float playerPositionX = sprite.getPosition().x;
	float playerPositionY = sprite.getPosition().y;
	mouse = sf::Vector2i(app->mapPixelToCoords(sf::Mouse::getPosition(*app)));
	sprite.setOrigin(16, 16);
	a = mouse.x - (playerPositionX);
	b = mouse.y - (playerPositionY);
	angle = -atan2(a, b) * 180 / PI;
	sprite.setRotation(angle);

	updateShield(frameClock);

	sf::Vector2f movement(0.0, 0.0);
	if (mIsMovingUp) {
		movement.y -= playerSpeed;
	}
	if (mIsMovingDown) {
		movement.y += playerSpeed;
	}
	if (mIsMovingLeft) {
		movement.x -= playerSpeed;
	}
	if (mIsMovingRight) {
		movement.x += playerSpeed;
	}
	if (getHitpoints() <= 0) {
		/* you dieded */
	}

	/* Goes in to check whether shooting flag is true or false */
	playerShoot();

	float positionPlayerX = sprite.getPosition().x;
	float positionPlayerY = sprite.getPosition().y;
	float playerSpeedX = (movement.x * frameClock + positionPlayerX);
	float playerSpeedY = (movement.y * frameClock + positionPlayerY);

	int collisionX = game->map->collision2(playerSpeedX, positionPlayerY, "player");
	int collisionY = game->map->collision2(positionPlayerX, playerSpeedY, "player");


	if (collisionX != 1 || collisionY != 1) {
		if (collisionX == 1) {
			movement.x = 0.0;
		}
		if (collisionY == 1) {
			movement.y = 0.0;
		}

		sprite.move(movement * frameClock);
		positionPlayerX = sprite.getPosition().x;
		positionPlayerY = sprite.getPosition().y;
	}
}


void Object::approachPath(float enemyPositionX, float enemyPositionY, float realPlayerPositionX, float realPlayerPositionY)
{
	clock += frameClock;
	int playerPositionX = playerPath[0].x;
	int playerPositionY = playerPath[0].y;

	if (abs(playerPositionX - enemyPositionX) < 2 && abs(playerPositionY - enemyPositionY) < 2) {
		playerPath.erase(playerPath.begin());
	}
	if (playerPath.size() > 0) {
		playerPositionX = playerPath[0].x;
		playerPositionY = playerPath[0].y;
		int collisionFlag = 1;
		float absoluteDistanceX = abs(playerPositionX - enemyPositionX);
		float absoluteDistanceY = abs(playerPositionY - enemyPositionY);
		float distanceFromPlayer = sqrt(absoluteDistanceX * 2 + absoluteDistanceY * 2);
		float enemySpeed = getEnemySpeed();
		float angle;

		sf::Vector2f enemyMovement(0.f, 0.f);

		float distanceX = playerPositionX - enemyPositionX;
		float distanceY = playerPositionY - enemyPositionY;


		if (distanceX > 0) {
			enemyMovement.x += enemySpeed;
		}
		if (distanceX < 0) {
			enemyMovement.x -= enemySpeed;
		}
		if (distanceY > 0) {
			enemyMovement.y += enemySpeed;
		}
		if (distanceY < 0) {
			enemyMovement.y -= enemySpeed;
		}


		float enemySpeedX = (enemyMovement.x * frameClock + enemyPositionX);
		float enemySpeedY = (enemyMovement.y * frameClock + enemyPositionY);
		int enemyCollisionX = game->map->collision2(enemySpeedX, enemyPositionY, "player");
		int enemyCollisionY = game->map->collision2(enemyPositionX, enemySpeedY, "player");

		if (enemyCollisionX != 1 || enemyCollisionY != 1) {
			collisionFlag = 0;
			if (enemyCollisionX == 1) {
				enemyMovement.x = 0.f;
			}
			if (enemyCollisionY == 1) {
				enemyMovement.y = 0.f;
			}
		}
		if (collisionFlag == 0) {
			float angleX = playerPath[playerPath.size() - 1].x - enemyPositionX;
			float angleY = playerPath[playerPath.size() - 1].y - enemyPositionY;
			//float angleX = realPlayerPositionX - enemyPositionX;
			//float angleY = realPlayerPositionY - enemyPositionY;
			angle = -atan2(angleX, angleY) * 180 / 3.141593;
			this->sprite.setOrigin(16, 16);
			this->sprite.setRotation(angle);
			this->sprite.move(enemyMovement * frameClock);

			if (isPlaying == false) {
				clock = 0.0;
				random = rand() % game->audio->footsteps.size();
				/* A hacky way to increase the volume relative to distance */
				game->audio->footsteps[random]->setVolume(MISC_VOLUME+30);
				game->audio->footsteps[random]->setPosition(-distanceX / 25, -distanceY / 25, 0);
				game->audio->footsteps[random]->play();
				isPlaying = true;
			} else if (isPlaying == true) {
				if (clock > 0.35) {
					isPlaying = false;
				}
			}
		}
	}
}


int Object::lineOfSight(int _x0, int _y0, int _x1, int _y1)
{
	int x0 = (_x0 - 10) / 32, y0 = (_y0 - 10) / 32, x1 = _x1 / 32, y1 = _y1 / 32;
	std::vector<sf::Vector2f> line = getLine(x0, y0, x1, y1);
	int lineOfSight = 1;
	for (int i = 0; i < line.size(); i++) {
		if (game->map->gridCollision(line[i].x, line[i].y) == 1) {
			lineOfSight = 0;
		}
	}

	x0 = (_x0 + 10) / 32, y0 = (_y0 + 10) / 32;
	std::vector<sf::Vector2f> line2 = getLine(x0, y0, x1, y1);
	for (int i = 0; i < line2.size(); i++) {
		if (game->map->gridCollision(line2[i].x, line2[i].y) == 1) {
			lineOfSight = 0;
		}
	}
	return lineOfSight;
	return lineOfSight;
}

std::vector<sf::Vector2f> Object::getLine(int x0, int y0, int x1, int y1)
{
	int sx, sy, dx, dy, err, e2;

	dx = abs(x1 - x0);
	dy = abs(y1 - y0);
	std::vector<sf::Vector2f> line;

	sx = (x0 < x1) ? 1 : -1;
	sy = (y0 < y1) ? 1 : -1;

	err = dx - dy;

	while (true) {
		sf::Vector2f point(x0, y0);
		line.push_back(point);

		if (x0 == x1 && y0 == y1) {
			break;
		}

		e2 = 2 * err;
		if (e2 > -dy) {
			err = err - dy;
			x0 = x0 + sx;
		}
		if (e2 < dx) {
			err = err + dx;
			y0 = y0 + sy;
		}
	}
	return line;
}

int Object::getDistanceBetweenTiles(int _x0, int _y0, int _x1, int _y1)
{
	int x0 = _x0, x1 = _x1, y0 = _y0, y1 = _y1;
	int tiles = 0;
	int dx = abs(x1 - x0);
	int dy = abs(y1 - y0);

	tiles = sqrt(dx * dx + dy * dy) / 32;

	return tiles;
}

/*
 * Weapon and projectile vectors reside in main Game class, so everything is
 * called through game->.
 */
void Object::playerShoot()
{
	if (playerShooting == true && shootingCooldown <= 0) {
		shootingCooldown = game->playerWeapons[game->heldWeapon]->attackSpeed;
		game->playerWeapons[game->heldWeapon]->setAmmo(1);
		game->ammo = game->playerWeapons[game->heldWeapon]->getAmmo();
		if (game->playerWeapons[game->heldWeapon]->getAmmo() <= 0) {
			game->audio->clickSound->play();
		} else {
			switch (game->playerWeapons[game->heldWeapon]->ammoType) {
			case 0:
				game->projectiles.push_back(BulletSprite(*game->bulletTexture, game->player->sprite.getPosition(),
					sf::Vector2i(app->mapPixelToCoords(sf::Mouse::getPosition(*app))),
					game->playerWeapons[game->heldWeapon]->spreadAngle));

				game->audio->bulletSound->setPosition(0,0,0);
				game->audio->bulletSound->play();

				break;
			case 1:
				game->projectiles.push_back(LaserSprite(*game->laserBeamTexture, game->player->sprite.getPosition(),
					sf::Vector2i(app->mapPixelToCoords(sf::Mouse::getPosition(*app))),
					game->playerWeapons[game->heldWeapon]->spreadAngle));

				game->audio->laserSound->play();

				break;
			case 2:
				for (int i = 0; i < game->playerWeapons[game->heldWeapon]->bullets; i++) {
					game->projectiles.push_back(PelletSprite(*game->pelletTexture, game->player->sprite.getPosition(),
						sf::Vector2i(app->mapPixelToCoords(sf::Mouse::getPosition(*app))),
						game->playerWeapons[game->heldWeapon]->spreadAngle));
				}

				game->audio->shotgunSound->play();

				break;
			case 3:
				game->projectiles.push_back(HeavyBulletSprite(*game->heavyBulletTexture, game->player->sprite.getPosition(),
					sf::Vector2i(app->mapPixelToCoords(sf::Mouse::getPosition(*app))),
					game->playerWeapons[game->heldWeapon]->spreadAngle));

				game->audio->sniperSound->play();

				break;
			default:
				std::cout << "heldWeapon fail: " << game->heldWeapon << std::endl;
				break;
			}
		}
	} else {
		shootingCooldown -= frameClock;
	}
}

void Object::enemyShoot(sf::Vector2i coords, float distanceX, float distanceY)
{
	if (cooldown <= 0) {
		setCooldown(0.5);
		game->enemyProjectiles.push_back(BulletSprite(*game->bulletTexture, sprite.getPosition(), coords, 15));
		game->audio->bulletSound->setPosition(-distanceX / 25, -distanceY / 25, 0);
		game->audio->bulletSound->play();
	}
	else cooldown -= frameClock;
}

void Object::updateShield(float frameClock)
{
	shieldTimeUntilRecharge += frameClock;
	if (getShieldpoints() < maxShieldPoints) {
		if (getShieldpoints() < 0) {
			setShieldpoints(0);
		}
		if (shieldTimeUntilRecharge > shieldRechargeDelay) {
			setShieldpoints(getShieldpoints() + 0.1 + ((maxShieldPoints - getShieldpoints()) / 200));
		}
	} else if (getShieldpoints() >= maxShieldPoints) {
		setShieldpoints(maxShieldPoints);
		shieldTimeUntilRecharge = 0.0;
	}
}

int Object::getHitpoints() const
{
	return mHitpoints;
}

void Object::setHitpoints(int hp)
{
	mHitpoints = hp;
}

float Object::getShieldpoints() const
{
	return mShieldpoints;
}

void Object::setShieldpoints(float shield)
{
	mShieldpoints = shield;
}

void Object::setDamage(float damage)
{
	float currentShieldpoints = getShieldpoints();
	int currentHitpoints = getHitpoints();
	if (currentShieldpoints <= 0) {
		for (int tmp = 0; tmp < 1 + rand() % 3; tmp++) {
			game->mapBlood.push_back(new BloodSmall(*game->blood8x8Texture, sprite.getPosition()));
		}
		int newHitpoints = currentHitpoints - damage;
		if (newHitpoints < 0) newHitpoints = 0;
		setHitpoints(newHitpoints);
	} else {
		float newShieldpoints = currentShieldpoints - damage;
		setShieldpoints(newShieldpoints);
	}
}

void Object::setMeleeDamage(float damage)
{
	meleeDamage = damage;
}

float Object::getMeleeDamage() const
{
	return meleeDamage;
}

void Object::setCooldown(float amount)
{
	cooldown = amount;
}

void Object::setAggro(float amount)
{
	aggro = amount;
}

float Object::getAggro() const
{
	return aggro;
}

void Object::updateAggro(float amount)
{
	aggro -= amount;
}

void Object::setValue(int givenValue)
{
	value = givenValue;
}

int Object::getValue() const
{
	return value;
}

float Object::getEnemySpeed() const
{
	return enemySpeed;
}

void Object::setEnemySpeed(float amount)
{
	enemySpeed = amount;
}

Player::Player(sf::Texture& objectTexture, sf::Vector2f coords) : Object(objectTexture)
{
	maxShieldPoints = 200.0;
	shieldRechargeDelay = 2.0;
	/*
	ObjectTex = new sf::Texture();
	ObjectTex->loadFromFile("media/ddos-dude-guns.png");
	*/
	sf::IntRect mPlayer(32 * 0, 32 * 0, 32, 32);
	//ObjectTex = Game::playerTexture;
	//sprite.setTexture(*ObjectTex);
	sprite.setTextureRect(mPlayer);
	sprite.setOrigin(16, 16);
	sprite.setPosition(coords.x, coords.y);

	setHitpoints(100);
	setShieldpoints(100.0);
	setMeleeDamage(0);

}

EnemyMelee::EnemyMelee(sf::Texture& objectTexture, sf::Vector2f coords) : Object(objectTexture)
{
	maxShieldPoints = 0.0;
	setValue(100);
	setEnemySpeed(75);
	/*
	ObjectTex = new sf::Texture();
	ObjectTex->loadFromFile("media/ddos-dude-guns.png");
	*/
	sf::IntRect mEnemy(32 * 3, 32 * 6, 32, 32);
	//ObjectTex = game->playerTexture;
	//sprite.setTexture(*ObjectTex);
	sprite.setTextureRect(mEnemy);
	sprite.setOrigin(16, 16);
	sprite.setPosition(coords.x, coords.y);

	setHitpoints(50);
	setShieldpoints(0.0);
	setMeleeDamage(1);
	setCooldown(0);

}

void EnemyMelee::update(float enemyPositionX, float enemyPositionY,
	float playerPositionX, float playerPositionY)

{
	if (getDistanceBetweenTiles(enemyPositionX, enemyPositionY, playerPositionX, playerPositionY) <= 7 || getAggro() > 0) {
		if (lineOfSight(enemyPositionX, enemyPositionY, playerPositionX, playerPositionY) == 1) {
			setAggro(5);
			approach(enemyPositionX, enemyPositionY, playerPositionX, playerPositionY);
			newPath = true;
		}
		else {
			if ((pathCooldown <= 0 && newPath) && getAggro() > 0) {
				sf::Vector2f coords(enemyPositionX, enemyPositionY);
				if (playerPath.size() > 0) {
					playerPath = game->testPath(coords);
				}
				else if (playerPath.size() <= 0) {
					playerPath = game->testPath(coords);
				}
				pathCooldown = 1;
				newPath = false;
			}
			else if (playerPath.size() > 0) {
				approachPath(enemyPositionX, enemyPositionY, playerPositionX, playerPositionY);
				if (playerPath.size() <= 0 && getAggro() > 0) newPath = true;
			}
		}
	}
	else if (getDistanceBetweenTiles(enemyPositionX, enemyPositionY, playerPositionX, playerPositionY) <= 10 && playerPath.size() >= 1) {
		approachPath(enemyPositionX, enemyPositionY, playerPositionX, playerPositionY);
	}
	else {
		newPath = true;
	}
	if (getAggro() > 0) updateAggro(frameClock);
	if (pathCooldown > 0) pathCooldown -= frameClock;

}

void EnemyMelee::approach(float enemyPositionX, float enemyPositionY,
	float playerPositionX, float playerPositionY)
{
	int collisionFlag = 1;
	float absoluteDistanceX = abs(playerPositionX - enemyPositionX);
	float absoluteDistanceY = abs(playerPositionY - enemyPositionY);
	float distanceFromPlayer = sqrt(absoluteDistanceX * 2 + absoluteDistanceY * 2);
	float enemySpeed = getEnemySpeed();
	float angle;
	sf::Vector2f enemyMovement(0.f, 0.f);
	if (((distanceFromPlayer < 20) && (distanceFromPlayer > 5)) || (getAggro() > 0)) {
		float distanceX = playerPositionX - enemyPositionX;
		float distanceY = playerPositionY - enemyPositionY;
		if (distanceX > 0) {
			enemyMovement.x += enemySpeed;
		}
		if (distanceX < 0) {
			enemyMovement.x -= enemySpeed;
		}
		if (distanceY > 0) {
			enemyMovement.y += enemySpeed;
		}
		if (distanceY < 0) {
			enemyMovement.y -= enemySpeed;
		}

		float enemySpeedX = (enemyMovement.x * frameClock + enemyPositionX);
		float enemySpeedY = (enemyMovement.y * frameClock + enemyPositionY);

		int enemyCollisionX = game->map->collision2(enemySpeedX, enemyPositionY, "player");
		int enemyCollisionY = game->map->collision2(enemyPositionX, enemySpeedY, "player");

		if (enemyCollisionX != 1 || enemyCollisionY != 1) {
			collisionFlag = 0;
			if (enemyCollisionX == 1) {
				enemyMovement.x = 0.f;
			}
			if (enemyCollisionY == 1) {
				enemyMovement.y = 0.f;
			}
		}

		// Start approaching when player is close enough to the enemy 
		if (((distanceFromPlayer < 20) && (distanceFromPlayer > 5) &&
			(collisionFlag == 0)) || (getAggro() > 0 && distanceFromPlayer > 5)) {
			sf::Vector2i playerCoords(playerPositionX, playerPositionY);
			angle = -atan2(distanceX, distanceY) * 180 / PI;
			this->sprite.setOrigin(16, 16);
			this->sprite.setRotation(angle);
			this->sprite.move(enemyMovement * frameClock);
		}
	}
}

EnemySoldier::EnemySoldier(sf::Texture& objectTexture, sf::Vector2f coords) : Object(objectTexture)
{
	maxShieldPoints = 0.0;
	setValue(100);
	setEnemySpeed(50);
	/*
	ObjectTex = new sf::Texture();
	ObjectTex->loadFromFile("media/ddos-dude-guns.png");
	*/
	sf::IntRect mEnemy(32 * 2, 32 * 9, 32, 32);
	//ObjectTex = game->playerTexture;
	//sprite.setTexture(*ObjectTex);
	sprite.setTextureRect(mEnemy);
	sprite.setOrigin(16, 16);
	sprite.setPosition(coords.x, coords.y);

	setHitpoints(50);
	setShieldpoints(0.0);
	setMeleeDamage(0.5);
	setCooldown(0);
	approachTiles = rand() % 3 + 1;

}

void EnemySoldier::update(float enemyPositionX, float enemyPositionY,
	float playerPositionX, float playerPositionY)

{
	if (getDistanceBetweenTiles(enemyPositionX, enemyPositionY, playerPositionX, playerPositionY) <= 7 || getAggro() > 0) {
		if (lineOfSight(enemyPositionX, enemyPositionY, playerPositionX, playerPositionY) == 1) {
			setAggro(5);
			approach(enemyPositionX, enemyPositionY, playerPositionX, playerPositionY);
			newPath = true;
		}
		else {
			if ((pathCooldown <= 0 && newPath) && getAggro() > 0) {
				sf::Vector2f coords(enemyPositionX, enemyPositionY);
				if (playerPath.size() > 0) {
					playerPath = game->testPath(coords);
				}
				else if (playerPath.size() <= 0) {
					playerPath = game->testPath(coords);
				}
				pathCooldown = 1;
				newPath = false;
			}
			else if (playerPath.size() > 0) {
				approachPath(enemyPositionX, enemyPositionY, playerPositionX, playerPositionY);
				if (playerPath.size() <= 0 && getAggro() > 0) newPath = true;
			}
		}
	}
	else if (getDistanceBetweenTiles(enemyPositionX, enemyPositionY, playerPositionX, playerPositionY) <= 10 && playerPath.size() >= 1) {
		approachPath(enemyPositionX, enemyPositionY, playerPositionX, playerPositionY);
	}
	if (getAggro() > 0) updateAggro(frameClock);
	if (pathCooldown > 0) pathCooldown -= frameClock;

}

void EnemySoldier::approach(float enemyPositionX, float enemyPositionY,
	float playerPositionX, float playerPositionY)
{
	clock += frameClock;
	int collisionFlag = 1;
	float distanceFromPlayer = getDistanceBetweenTiles(enemyPositionX, enemyPositionY, playerPositionX, playerPositionY);
	float enemySpeed = getEnemySpeed();
	float angle;
	sf::Vector2f enemyMovement(0.f, 0.f);
	float distanceX = playerPositionX - enemyPositionX;
	float distanceY = playerPositionY - enemyPositionY;

	if (distanceFromPlayer < 4) {
			if (distanceFromPlayer <= approachTiles) {
			sf::Vector2i playerCoords(playerPositionX, playerPositionY);
			angle = -atan2(distanceX, distanceY) * 180 / PI;
			this->sprite.setOrigin(16, 16);
			this->sprite.setRotation(angle);
			enemyShoot(playerCoords, distanceX, distanceY);
		}
	
		
		else if (distanceFromPlayer < 4 && distanceFromPlayer > approachTiles){
			if (distanceX > 0) {
				enemyMovement.x += enemySpeed;
			}
			if (distanceX < 0) {
				enemyMovement.x -= enemySpeed;
			}
			if (distanceY > 0) {
				enemyMovement.y += enemySpeed;
			}
			if (distanceY < 0) {
				enemyMovement.y -= enemySpeed;
			}

			float enemySpeedX = (enemyMovement.x * frameClock + enemyPositionX);
			float enemySpeedY = (enemyMovement.y * frameClock + enemyPositionY);

			int enemyCollisionX = game->map->collision2(enemySpeedX, enemyPositionY, "player");
			int enemyCollisionY = game->map->collision2(enemyPositionX, enemySpeedY, "player");

			if (enemyCollisionX != 1 || enemyCollisionY != 1) {
				collisionFlag = 0;
				if (enemyCollisionX == 1) {
					enemyMovement.x = 0.f;
				}
				if (enemyCollisionY == 1) {
					enemyMovement.y = 0.f;
				}
			}
			if (collisionFlag == 0) {
				sf::Vector2i playerCoords(playerPositionX, playerPositionY);
				angle = -atan2(distanceX, distanceY) * 180 / PI;
				this->sprite.setOrigin(16, 16);
				this->sprite.setRotation(angle);
				this->sprite.move(enemyMovement * frameClock);
				enemyShoot(playerCoords, distanceX, distanceY);
			}
		}
	}
	
	else if ((distanceFromPlayer < 8 && distanceFromPlayer > 4) || (getAggro() > 0)) {

		if (distanceX > 0) {
			enemyMovement.x += enemySpeed;
		}
		if (distanceX < 0) {
			enemyMovement.x -= enemySpeed;
		}
		if (distanceY > 0) {
			enemyMovement.y += enemySpeed;
		}
		if (distanceY < 0) {
			enemyMovement.y -= enemySpeed;
		}

		float enemySpeedX = (enemyMovement.x * frameClock + enemyPositionX);
		float enemySpeedY = (enemyMovement.y * frameClock + enemyPositionY);

		int enemyCollisionX = game->map->collision2(enemySpeedX, enemyPositionY, "player");
		int enemyCollisionY = game->map->collision2(enemyPositionX, enemySpeedY, "player");

		if (enemyCollisionX != 1 || enemyCollisionY != 1) {
			collisionFlag = 0;
			if (enemyCollisionX == 1) {
				enemyMovement.x = 0.f;
			}
			if (enemyCollisionY == 1) {
				enemyMovement.y = 0.f;
			}
		}

		// Start approaching when player is close enough to the enemy 
		if (((distanceFromPlayer < 8) && (distanceFromPlayer > 4) &&
			(collisionFlag == 0)) || (getAggro() > 0)) {
			sf::Vector2i playerCoords(playerPositionX, playerPositionY);
			angle = -atan2(distanceX, distanceY) * 180 / PI;
			this->sprite.setOrigin(16, 16);
			this->sprite.setRotation(angle);
			this->sprite.move(enemyMovement * frameClock);
			enemyShoot(playerCoords, distanceX, distanceY);
			
			if (isPlaying == false) {
				clock = 0.0;
				random = rand() % game->audio->footsteps.size();
				/* A hacky way to increase the volume relative to distance */
				game->audio->footsteps[random]->setVolume(MISC_VOLUME+30);
				game->audio->footsteps[random]->setPosition(-distanceX / 25, -distanceY / 25, 0);
				game->audio->footsteps[random]->play();
				isPlaying = true;
			} else if (isPlaying == true) {
				if (clock > 0.35) {
					isPlaying = false;
				}
			}
		}
	}
}

ValuableLow::ValuableLow(sf::Texture& objectTexture, sf::Vector2f coords) : Object(objectTexture)
{
	setValue(100);
	sprite.setPosition(coords.x + rand() % 16, coords.y + rand() % 16);
	sprite.setColor(sf::Color(207, 83, 0));
}

ValuableMed::ValuableMed(sf::Texture& objectTexture, sf::Vector2f coords) : Object(objectTexture)
{
	setValue(250);
	sprite.setPosition(coords.x + rand() % 16, coords.y + rand() % 16);
	sprite.setColor(sf::Color(194, 194, 163));
}

ValuableHigh::ValuableHigh(sf::Texture& objectTexture, sf::Vector2f coords) : Object(objectTexture)
{
	setValue(500);
	sprite.setPosition(coords.x + rand() % 16, coords.y + rand() % 16);
	sprite.setColor(sf::Color(255, 204, 0));
}

BloodSmall::BloodSmall(sf::Texture& objectTexture, sf::Vector2f coords) : Object(objectTexture)
{
	/* Number of blood variations in blood8x8Texture */
	int variation = rand() % 8;
	sf::IntRect mBlood(variation * 8, 0, 8, 8);
	sprite.setTextureRect(mBlood);
	sprite.setOrigin(8, 8);
	sprite.setRotation(rand() % 360);
	sprite.setPosition(coords.x + (rand() % 24 - 12), coords.y + (rand() % 24 - 12));
}

BloodMedium::BloodMedium(sf::Texture& objectTexture, sf::Vector2f coords) : Object(objectTexture)
{
	/* Number of blood variations in blood16x16Texture */
	int variation = rand() % 8;
	sf::IntRect mBlood(variation * 16, 0, 16, 16);
	sprite.setTextureRect(mBlood);
	sprite.setOrigin(8, 8);
	sprite.setRotation(rand() % 360);
	sprite.setPosition(coords.x + (rand() % 16 - 8), coords.y + (rand() % 16 - 8));
}

HealthBar::HealthBar(sf::Texture& objectTexture) : Object(objectTexture)
{

}

ShieldBar::ShieldBar(sf::Texture& objectTexture) : Object(objectTexture)
{

}
