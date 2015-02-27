/*
 * Project Name
 * 2015 © Project Team (see: LICENSE)
 */

#include <SFML/Graphics.hpp>
#include "object.hpp"
#include "main.hpp"
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
	mPlayerSpr->move(movement * TimePerFrame.asSeconds());
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
