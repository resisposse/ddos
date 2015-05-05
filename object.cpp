/*
* Project Name
* 2015 © Project Team (see: LICENSE)
*/

#include <SFML/Graphics.hpp>
#include <iostream>
#include "object.hpp"
#include "main.hpp"
#include "map.hpp"
#include <math.h>

const float Object::PlayerSpeed = 100.f;
sf::Time timeSinceLastUpdate = sf::Time::Zero;
const sf::Time Object::TimePerFrame = sf::seconds(1.f / 60.f);
sf::Sprite *mPlayerSpr;
sf::Sprite *mEnemySpr;

Object::Object()
{
	mIsMovingUp = false;
	mIsMovingDown = false;
	mIsMovingRight = false;
	mIsMovingLeft = false;
}
/*
Object::~Object()
{
	delete ObjectTex;
	delete TextureCursor;
	delete spriteCursor;
}
*/

void Object::run()
{
	float angle;
	double a, b;

	mouse = sf::Vector2i(app->mapPixelToCoords(sf::Mouse::getPosition(*app)));
	float positionPlayerX = sprite.getPosition().x;
	float positionPlayerY = sprite.getPosition().y;
	sprite.setOrigin(16, 16);

	a = mouse.x - (positionPlayerX);
	b = mouse.y - (positionPlayerY);
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
	}
}

void Object::update(sf::Time TimePerFrame, float enemyPosX, float enemyPosY, float playerPosX, float playerPosY)
{
	float positionPlayerX = playerPosX;
	float positionPlayerY = playerPosY;
	float positionEnemyX = enemyPosX;
	float positionEnemyY = enemyPosY;

	/* Make enemies approach the player */
	approach(positionEnemyX, positionEnemyY, positionPlayerX, positionPlayerY);
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
	}
	else if (key == sf::Keyboard::S) {
		mIsMovingDown = isPressed;
	}
	else if (key == sf::Keyboard::A) {
		mIsMovingLeft = isPressed;
	}
	else if (key == sf::Keyboard::D) {
		mIsMovingRight = isPressed;
	}
}

float Object::updateCollisionY()
{
	std::cout << "updatecollision old y; " << objectY[0];
	float Y = objectY[1];
	std::cout << "updatecollision y: " << Y << std::endl;
	return Y;
}

/* Handle enemy movement by approaching the player and check for collisions */
void Object::approach(float positionEnemyX, float positionEnemyY, float positionPlayerX, float positionPlayerY)
{
	float enemySpeed = 50;
	int collisionFlag = 1;
	sf::Vector2f enemyMovement(0.f, 0.f);
	float distanceX, distanceY, distance, angle;
	double x, y;

	/* abs = absolute value */
	distanceX = abs(positionPlayerX - positionEnemyX);
	distanceY = abs(positionPlayerY - positionEnemyY);
	float distanceXTest = positionPlayerX - positionEnemyX;
	float distanceYTest = positionPlayerY - positionEnemyY;

	if (distanceXTest > 0) {
		enemyMovement.x += enemySpeed;
	}
	if (distanceXTest < 0) {
		enemyMovement.x -= enemySpeed;
	}
	if (distanceYTest > 0) {
		enemyMovement.y += enemySpeed;
	}
	if (distanceYTest < 0) {
		enemyMovement.y -= enemySpeed;
	}

	distance = sqrt(distanceX * 2 + distanceY * 2);
	float enemySpeedX = (enemyMovement.x * TimePerFrame.asSeconds() + positionEnemyX);
	float enemySpeedY = (enemyMovement.y * TimePerFrame.asSeconds() + positionEnemyY);
	int enemyCollisionX = game->collision(enemySpeedX, positionEnemyY, "player");
	int enemyCollisionY = game->collision(positionEnemyX, enemySpeedY, "player");

	if (enemyCollisionX != 1 || enemyCollisionY != 1) {
		collisionFlag = 0;
		if (enemyCollisionX == 1) {
			enemyMovement.x = 0.f;
		}
		if (enemyCollisionY == 1) {
			enemyMovement.y = 0.f;
		}
	}

	if (distance < 20 && distance > 5 && collisionFlag == 0) {
		this->sprite.setOrigin(16, 16);
		x = positionPlayerX - (positionEnemyX);
		y = positionPlayerY - (positionEnemyY);
		angle = -atan2(x, y) * 180 / 3.141593;
		this->sprite.setRotation(angle);
		this->sprite.move(enemyMovement * TimePerFrame.asSeconds());
	}
}

int Object::getHitpoints() const{
	return mHitpoints;
}

void Object::setHitpoints(int hp){
	mHitpoints = hp;
}

Player::Player()
{
	ObjectTex = new sf::Texture();
	ObjectTex->loadFromFile("media/ddos-dude-guns.png");
	sf::IntRect mPlayer(32 * 0, 32 * 0, 32, 32);

	sprite.setTexture(*ObjectTex);
	sprite.setTextureRect(mPlayer);
	sprite.setOrigin(16, 16);
	sprite.setPosition(500, 200);

	setHitpoints(100);
}
/*
Player::~Player(){
	delete mPlayerSpr;
}
*/
EnemyMelee::EnemyMelee()
{
	ObjectTex = new sf::Texture();
	ObjectTex->loadFromFile("media/ddos-dude-guns.png");
	sf::IntRect mEnemy(32 * 0, 32 * 0, 32, 32);

	sprite.setTexture(*ObjectTex);
	sprite.setTextureRect(mEnemy);
	sprite.setOrigin(16, 16);
	sprite.setPosition(rand() % 300 + 400, rand() % 300 + 200);

	setHitpoints(50);
}
/*
EnemyMelee::~EnemyMelee() {
	delete mEnemySpr;
}
*/