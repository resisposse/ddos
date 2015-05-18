/*
 * Dark Domains Of Space
 * 2015 © Project Team (see: LICENSE)
 */

#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include "map.hpp"
#include "projectile.hpp"
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

void Object::update(float enemyPositionX, float enemyPositionY,
                    float playerPositionX, float playerPositionY)
{
	approach(enemyPositionX, enemyPositionY, playerPositionX, playerPositionY);
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
	angle = -atan2(a, b) * 180 / 3.141593;
	sprite.setRotation(angle);

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

	int collisionX = game->map->collision(playerSpeedX, positionPlayerY, "player");
	int collisionY = game->map->collision(positionPlayerX, playerSpeedY, "player");

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

/* Handle enemy movement by approaching the player and check for collisions */
void Object::approach(float enemyPositionX, float enemyPositionY,
                      float playerPositionX, float playerPositionY)
{
	int collisionFlag = 1;
	float absoluteDistanceX = abs(playerPositionX - enemyPositionX);
	float absoluteDistanceY = abs(playerPositionY - enemyPositionY);
	float distanceFromPlayer = sqrt(absoluteDistanceX * 2 + absoluteDistanceY * 2);
	float enemySpeed = 50;
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
		int enemyCollisionX = game->map->collision(enemySpeedX, enemyPositionY, "player");
		int enemyCollisionY = game->map->collision(enemyPositionX, enemySpeedY, "player");

		if (enemyCollisionX != 1 || enemyCollisionY != 1) {
			collisionFlag = 0;
			if (enemyCollisionX == 1) {
				enemyMovement.x = 0.f;
			}
			if (enemyCollisionY == 1) {
				enemyMovement.y = 0.f;
			}
		}

		/* Start approaching when player is close enough to the enemy */
		if (((distanceFromPlayer < 20) && (distanceFromPlayer > 5) &&
		    (collisionFlag == 0)) || (getAggro() > 0)) {
			if (getAggro() > 0) {
				aggro -= frameClock;
			}
			sf::Vector2i playerCoords(playerPositionX, playerPositionY);
			angle = -atan2(distanceX, distanceY) * 180 / 3.141593;
			this->sprite.setOrigin(16, 16);
			this->sprite.setRotation(angle);
			this->sprite.move(enemyMovement * frameClock);
			enemyShoot(playerCoords);
		}
	}
}

/*
 * Weapon and projectile vectors reside in main Game class, so everything is
 * called through game->.
 */
void Object::playerShoot()
{
	if (playerShooting == true && shootingCooldown <= 0) {
		shootingCooldown = game->weapons[game->heldWeapon].attackSpeed;
		game->ammo = game->weapons[game->heldWeapon].getAmmo();
		game->weapons[game->playerWeapons[game->heldWeapon].weaponPosition].setAmmo(1);
		switch (game->playerWeapons[game->heldWeapon].ammoType) {
		case 0:
			game->projectiles.push_back(BulletSprite(*game->bulletTexture, game->player->sprite.getPosition(),
			                                         sf::Vector2i(app->mapPixelToCoords(sf::Mouse::getPosition(*app))),
			                                         game->playerWeapons[game->heldWeapon].spreadAngle));
			break;
		case 1:
			game->projectiles.push_back(LaserSprite(*game->laserBeamTexture, game->player->sprite.getPosition(),
			                                        sf::Vector2i(app->mapPixelToCoords(sf::Mouse::getPosition(*app))),
			                                        game->playerWeapons[game->heldWeapon].spreadAngle));
			break;
		case 2:
			for (int i = 0; i < game->playerWeapons[game->heldWeapon].bullets; i++) {
				game->projectiles.push_back(PelletSprite(*game->pelletTexture, game->player->sprite.getPosition(),
				                                         sf::Vector2i(app->mapPixelToCoords(sf::Mouse::getPosition(*app))),
				                                         game->playerWeapons[game->heldWeapon].spreadAngle));
			}
			break;
		default:
			std::cout << "heldWeapon fail: " << game->heldWeapon << std::endl;
			break;
		}
	} else {
		shootingCooldown -= frameClock;
	}
}

void Object::enemyShoot(sf::Vector2i coords)
{
	if (cooldown <= 0) {
		setCooldown(0.5);
		game->enemyProjectiles.push_back(BulletSprite(*game->bulletTexture, sprite.getPosition(), coords, 15));
	}
	else cooldown -= frameClock;
}

int Object::getHitpoints() const
{
	return mHitpoints;
}

void Object::setHitpoints(int hp)
{
	mHitpoints = hp;
}

void Object::setDamage(float damage)
{
	int currentHitpoints = getHitpoints();
	int newHitpoints = currentHitpoints - damage;
	if (newHitpoints < 0) newHitpoints = 0;
	setHitpoints(newHitpoints);
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

Player::Player(sf::Texture& objectTexture, sf::Vector2f coords) : Object(objectTexture)
{
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
	setMeleeDamage(0);
}

EnemyMelee::EnemyMelee(sf::Texture& objectTexture, sf::Vector2f coords) : Object(objectTexture)
{
	/*
	ObjectTex = new sf::Texture();
	ObjectTex->loadFromFile("media/ddos-dude-guns.png");
	*/
	sf::IntRect mEnemy(32 * 0, 32 * 0, 32, 32);
	//ObjectTex = game->playerTexture;
	//sprite.setTexture(*ObjectTex);
	sprite.setTextureRect(mEnemy);
	sprite.setOrigin(16, 16);
	sprite.setPosition(coords.x, coords.y);

	setHitpoints(50);
	setMeleeDamage(0.5);
	setCooldown(0);
}

HealthBar::HealthBar(sf::Texture& objectTexture) : Object(objectTexture)
{

}
