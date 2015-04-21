/*
 * Project Name
 * 2015 © Project Team (see: LICENSE)
 */

#include "projectileSprite.hpp"

ProjectileSprite::ProjectileSprite(sf::Texture& projectileTexture, sf::Vector2f startPosition, sf::Vector2i mousePosition)
{
	float PI = 3.14159265359;
	int magnitude;
	float angle;
	sf::Vector2f difference;
	sf::Vector2u spriteSize = projectileTexture.getSize();
	position = startPosition;
	speed = 300;

	sprite.setTexture(projectileTexture);
	sprite.setScale(0.5, 0.5);
	sprite.setOrigin(sf::Vector2f(spriteSize.x / 2, spriteSize.y / 2));
	sprite.setPosition(position);

	/* Calculate the direction the bullet will travel */
	difference.x = position.x - mousePosition.x;
	difference.y = position.y - mousePosition.y;
	magnitude = sqrt(pow(difference.x, 2) + pow(difference.y, 2));
	normalized.x = difference.x / magnitude;
	normalized.y = difference.y / magnitude;

	/* Face sprite in the direction it's travelling */
	angle = atan2(difference.y, difference.x) * 180 / PI;
	sprite.setRotation(angle);
}

void ProjectileSprite::update(float second)
{
	/* Calculate sprites velocity and move it */
	xVelocity = -normalized.x * speed * second;
	yVelocity = -normalized.y * speed * second;
	sprite.move(sf::Vector2f(xVelocity, yVelocity));
	position = sprite.getPosition();
}

/* Undecided if we need this function
bool ProjectileSprite::outsideWindow()
{
	// check if sprite is outside of the window
	return position.x < 0 || position.x > WINDOW_WIDTH || position.y < 0 || position.y > WINDOW_HEIGHT;
}
*/

BulletSprite::BulletSprite(sf::Texture& projectileTexture, sf::Vector2f startPosition, sf::Vector2i mousePosition)
                           : ProjectileSprite(projectileTexture, startPosition, mousePosition)
{
	speed = 300;
}

LaserSprite::LaserSprite(sf::Texture& projectileTexture, sf::Vector2f startPosition, sf::Vector2i mousePosition)
                         : ProjectileSprite(projectileTexture, startPosition, mousePosition)
{
	speed = 1000;
}
