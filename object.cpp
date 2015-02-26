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
sf::Sprite mPlayer;

Object::Object()
{
	mIsMovingUp = false;
	mIsMovingDown = false;
	mIsMovingRight = false;
	mIsMovingLeft = false;
	mTexture.loadFromFile("media/Eagle.png");
	mTexture.setSmooth(true);
	mPlayer.setTexture(mTexture);
	mPlayer.setPosition(100, 100);
}

void Object::run()
{
	float angle;
	double a, b;

	mouse = sf::Mouse::getPosition(*app);
	float positionX = mPlayer.getPosition().x;
	float positionY = mPlayer.getPosition().y;
	mPlayer.setOrigin(24, 32);
	a = mouse.x - (positionX);
	b = mouse.y - (positionY);
	angle = -atan2(a, b) * 180 / 3.14;
	mPlayer.setRotation(180 + angle);

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
	mPlayer.move(movement * TimePerFrame.asSeconds());
}

void Object::render()
{
	app->draw(mPlayer);
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
