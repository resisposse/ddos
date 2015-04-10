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

Object::Object()
{
	mIsMovingUp = false;
	mIsMovingDown = false;
	mIsMovingRight = false;
	mIsMovingLeft = false;

	mTexture = new sf::Texture();
	mTexture->loadFromFile("media/ddos-dude-guns.png");

	sf::IntRect mPlayer(32 * 0, 32 * 0, 32, 32);
	mPlayerSpr = new sf::Sprite(*mTexture, mPlayer);
	mPlayerSpr->setPosition(500, 200);
}

Object::~Object()
{
	delete mTexture;
	delete mPlayerSpr;
}

void Object::run()
{
	float angle;
	double a, b;

	mouse = sf::Vector2i(app->mapPixelToCoords(sf::Mouse::getPosition(*app)));
	float positionX = mPlayerSpr->getPosition().x;
	float positionY = mPlayerSpr->getPosition().y;
	mPlayerSpr->setOrigin(16, 16);
	a = mouse.x - (positionX);
	b = mouse.y - (positionY);
	angle = -atan2(a, b) * 180 / 3.141593;
	mPlayerSpr->setRotation(angle);

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
	case sf::Event::KeyPressed:
		handlePlayerInput(event.key.code, true);
		break;
	case sf::Event::KeyReleased:
		handlePlayerInput(event.key.code, false);
		break;
	case sf::Event::Closed:
		app->close();
		break;
	}
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
	float positionX = mPlayerSpr->getPosition().x;
	float positionY = mPlayerSpr->getPosition().y;
	float testX = (movement.x * TimePerFrame.asSeconds() + positionX);
	float testY = (movement.y * TimePerFrame.asSeconds() + positionY);
	

	int collisionX = game->collision(testX, positionY, "player");
	int collisionY = game->collision(positionX, testY, "player");

	if (collisionX != 1 || collisionY != 1) {
		if (collisionX == 1) {
			movement.x = 0.f;
		}
		if (collisionY == 1) {
			movement.y = 0.f;
		}

		float positionX = movement.x* TimePerFrame.asSeconds();
		float positionY = movement.y * TimePerFrame.asSeconds();
		mPlayerSpr->move(movement * TimePerFrame.asSeconds());
		positionX = mPlayerSpr->getPosition().x;
		positionY = mPlayerSpr->getPosition().y;
		updatePosition(positionX, positionY);
	}

}

void Object::render()
{
	app->draw(*mPlayerSpr);
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

void Object::updatePosition(float X, float Y) {
	if (objectX[2] == NULL) {
		if (objectX[1] == NULL) {
			if (objectX[0] == NULL) {
				objectX[0] = X;
			}
			else if (objectX[0] != NULL) {
				objectX[1] = objectX[0];
				objectX[0] = X;
			}
		}
		else if (objectX[1] != NULL) {
			objectX[2] = objectX[1];
			objectX[1] = objectX[0];
			objectX[0] = X;
		}
	}
	else if (objectX[2] != NULL) {
		objectX[2] = objectX[1];
		objectX[1] = objectX[0];
		objectX[0] = X;
	}


	if (objectY[2] == NULL) {
		if (objectY[1] == NULL) {
			if (objectY[0] == NULL) {
				objectY[0] = Y;
			}
			else if (objectY[0] != NULL) {
				objectY[1] = objectY[0];
				objectY[0] = Y;
			}
		}
		else if (objectY[1] != NULL) {
			objectY[2] = objectY[1];
			objectY[1] = objectY[0];
			objectY[0] = Y;
		}
	}
	else if (objectY[2] != NULL) {
		objectY[2] = objectY[1];
		objectY[1] = objectY[0];
		objectY[0] = Y;
	}

}

//Check if we need this function. This function does not in use.
float Object::updateCollisionX(float X, float Y) {
	std::cout << "updatecollision, objectx 0: " << objectX[0] << " objectx 1: " << objectX[1] << " objectx 2: " << objectX[2] << std::endl;
	std::cout << "updatecollision, objecty 0: " << objectY[0] << " objecty 1: " << objectY[1] << " objecty 2: " << objectY[2] << std::endl;

	float newPositionX = objectX[0] - objectX[1];
	float newPositionY = objectY[0] - objectY[1];
	float parempiX = 0;
	float parempiY = 0;

	if (newPositionX > 0) {
		parempiX = X - (2 * newPositionX);
	}
	else if (newPositionX < 0) {
		parempiX = X - (2 * newPositionX);
	}
	else if (newPositionX == 0) {
		parempiX = objectX[0];
	}

	if (newPositionY > 0) {
		parempiY = objectY[0] - (2 * newPositionY);
	}
	else if (newPositionY < 0) {
		parempiY = objectY[0] - (2 * newPositionY);
	}
	else if (newPositionY == 0) {
		parempiY = objectY[0];
	}

	mPlayerSpr->setPosition(parempiX, parempiY);
	std::cout << "New position X: " << parempiX << " New position Y: " << parempiY << std::endl;

	return 0;
}

float Object::updateCollisionY() {

	std::cout << "updatecollision old y; " << objectY[0];
	float Y = objectY[1];
	std::cout << "updatecollision y: " << Y << std::endl;

	return Y;
}