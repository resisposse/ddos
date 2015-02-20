/*
 * Project Name
 * 2015 © Project Team (see: LICENSE)
 */

#include <SFML/Graphics.hpp>
#include "object.hpp"
#include "main.hpp"

const float Object::PlayerSpeed = 100.f;
const sf::Time Object::TimePerFrame = sf::seconds(1.f / 60.f);

Object::Object()
{
	mIsMovingUp = false;
	mIsMovingDown = false;
	mIsMovingRight = false;
	mIsMovingLeft = false;
	mTexture.loadFromFile("Eagle.png");
	mPlayer.setTexture(mTexture);
	mPlayer.setPosition(100, 100);
}

void Object::run()
{
	/*
	 * TODO: Ajanlaskumenetelmä pitää muuttaa.
	 * Toimii hyvin yksinään, mutta muun ohjelman kanssa kusee.
	 */

	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
		sf::Time elapsedTime = clock.restart();
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
