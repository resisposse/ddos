/*
 * Dark Domains Of Space
 * 2015 © Project Team (see: LICENSE)
 */

#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include "object.hpp"
#include "main.hpp"

const float Object::PlayerSpeed = 100.f;
sf::Time timeSinceLastUpdate = sf::Time::Zero;
const sf::Time Object::TimePerFrame = sf::seconds(1.f / 60.f);

Object::Object(sf::Texture& objectTexture)
{
	mIsMovingUp = false;
	mIsMovingDown = false;
	mIsMovingRight = false;
	mIsMovingLeft = false;

	//sf::IntRect mPlayer(32 * 0, 32 * 0, 32, 32);
	sprite.setTexture(objectTexture);
	//sprite.setTextureRect(mPlayer);
	//sprite.setOrigin(16, 16);
}

void Object::run()
{
	float angle;
	double a, b;

	mouse = sf::Vector2i(app->mapPixelToCoords(sf::Mouse::getPosition(*app)));
	float playerPositionX = sprite.getPosition().x;
	float playerPositionY = sprite.getPosition().y;
	sprite.setOrigin(16, 16);

	a = mouse.x - (playerPositionX);
	b = mouse.y - (playerPositionY);
	angle = -atan2(a, b) * 180 / 3.141593;
	sprite.setRotation(angle);

	sf::Time elapsedTime = timer.restart();
	timeSinceLastUpdate += elapsedTime;
	while (timeSinceLastUpdate > TimePerFrame) {
		timeSinceLastUpdate -= TimePerFrame;
		update(TimePerFrame);
	}
}

void Object::processEvent(sf::Event event)
{
	switch (event.type) {
	case sf::Event::KeyPressed: {
		handlePlayerInput(event.key.code, true);
		break;
	}
	case sf::Event::KeyReleased: {
		handlePlayerInput(event.key.code, false);
		break;
	}
	case sf::Event::Closed: {
		app->close();
		break;
	}
	default: {
		break;
	}
	}
}

void Object::update(float enemyPositionX, float enemyPositionY,
                    float playerPositionX, float playerPositionY)
{
	approach(enemyPositionX, enemyPositionY, playerPositionX, playerPositionY);
}

void Object::update(sf::Time TimePerFrame)
{
	sf::Vector2f movement(0.f, 0.f);
	if (mIsMovingUp) {
		movement.y -= PlayerSpeed;
	}
	if (mIsMovingDown) {
		movement.y += PlayerSpeed;
	}
	if (mIsMovingLeft) {
		movement.x -= PlayerSpeed;
	}
	if (mIsMovingRight) {
		movement.x += PlayerSpeed;
	}
	if (getHitpoints() <= 0) {
		std::cout << "you dieded" << std::endl;
	}

	float positionPlayerX = sprite.getPosition().x;
	float positionPlayerY = sprite.getPosition().y;
	float playerSpeedX = (movement.x * TimePerFrame.asSeconds() + positionPlayerX);
	float playerSpeedY = (movement.y * TimePerFrame.asSeconds() + positionPlayerY);

	int collisionX = game->collision(playerSpeedX, positionPlayerY, "player");
	int collisionY = game->collision(positionPlayerX, playerSpeedY, "player");

	if (collisionX != 1 || collisionY != 1) {
		if (collisionX == 1) {
			movement.x = 0.f;
		}
		if (collisionY == 1) {
			movement.y = 0.f;
		}

		sprite.move(movement * TimePerFrame.asSeconds());
		positionPlayerX = sprite.getPosition().x;
		positionPlayerY = sprite.getPosition().y;
	}
}

void Object::handlePlayerInput(sf::Keyboard::Key key, bool isPressed)
{
	if (key == sf::Keyboard::W) {
		mIsMovingUp = isPressed;
	} else if (key == sf::Keyboard::S) {
		mIsMovingDown = isPressed;
	} else if (key == sf::Keyboard::A) {
		mIsMovingLeft = isPressed;
	} else if (key == sf::Keyboard::D) {
		mIsMovingRight = isPressed;
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
	if ((distanceFromPlayer < 20 && distanceFromPlayer > 5) || (getAggro() > 0)) {
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

		float enemySpeedX = (enemyMovement.x * TimePerFrame.asSeconds() + enemyPositionX);
		float enemySpeedY = (enemyMovement.y * TimePerFrame.asSeconds() + enemyPositionY);
		int enemyCollisionX = game->collision(enemySpeedX, enemyPositionY, "player");
		int enemyCollisionY = game->collision(enemyPositionX, enemySpeedY, "player");

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
		if (((distanceFromPlayer < 20) && (distanceFromPlayer > 5)) &&
		    ((collisionFlag == 0) || (getAggro() > 0))) {
			if (getAggro() > 0) {
				aggro -= frameClock;
			}
			sf::Vector2i playerCoords(playerPositionX, playerPositionY);
			angle = -atan2(distanceX, distanceY) * 180 / 3.141593;
			this->sprite.setOrigin(16, 16);
			this->sprite.setRotation(angle);
			this->sprite.move(enemyMovement * TimePerFrame.asSeconds());
			enemyShoot(playerCoords);
		}
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
