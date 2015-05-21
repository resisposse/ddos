/*
 * Dark Domains Of Space
 * 2015 © Project Team (see: LICENSE)
 */

#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include "weapon.hpp"
#include "projectile.hpp"
#include "globals.hpp"

ProjectileSprite::ProjectileSprite(sf::Texture& projectileTexture, sf::Vector2f startPosition,
                                   sf::Vector2i mousePosition, int inaccuracy)
{
	float b;
	int magnitude, magnitudeTmp;
	float angle, spreadDist;
	sf::Vector2f difference, differenceTmp;
	sf::Vector2u spriteSize = projectileTexture.getSize();
	position = startPosition;
	speed = 300;
	damage = 10;

	sprite.setTexture(projectileTexture);
	sprite.setScale(0.5, 0.5);
	sprite.setOrigin(sf::Vector2f(spriteSize.x / 2, spriteSize.y / 2));
	sprite.setPosition(position);

	/* Calculate spread and the direction the bullet will travel */
	differenceTmp.x = position.x - mousePosition.x;
	differenceTmp.y = position.y - mousePosition.y;
	magnitudeTmp = sqrt(pow(differenceTmp.x, 2) + pow(differenceTmp.y, 2));

	spreadDist = tan(((rand() % inaccuracy - (inaccuracy / 2))) * PI / 180.0) * magnitudeTmp;
	b = 90 - (90.0 - (atan(differenceTmp.y / differenceTmp.x) * 180.0/PI));
	if (b <= 0) {
		difference.x = position.x - (mousePosition.x + (sin(b * PI / 180) * spreadDist));
		difference.y = position.y - (mousePosition.y - (cos(b * PI / 180) * spreadDist));
	} else if (b > 0) {
		difference.x = position.x - (mousePosition.x - (sin(b * PI / 180) * spreadDist));
		difference.y = position.y - (mousePosition.y + (cos(b * PI / 180) * spreadDist));
	}
	magnitude = sqrt(pow(difference.x, 2) + pow(difference.y, 2));
	normalized.x = (difference.x / magnitude);
	normalized.y = (difference.y / magnitude);

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

int ProjectileSprite::getDamage()
{
	return damage;
}

/* Undecided if we need this function
bool ProjectileSprite::outsideWindow()
{
	// check if sprite is outside of the window
	return position.x < 0 || position.x > WINDOW_WIDTH || position.y < 0 || position.y > WINDOW_HEIGHT;
}
*/

BulletSprite::BulletSprite(sf::Texture& projectileTexture, sf::Vector2f startPosition, sf::Vector2i mousePosition, int inaccuracy)
                           : ProjectileSprite(projectileTexture, startPosition, mousePosition, inaccuracy)
{
	speed = 500;
	damage = 10;
}

LaserSprite::LaserSprite(sf::Texture& projectileTexture, sf::Vector2f startPosition, sf::Vector2i mousePosition, int inaccuracy)
                         : ProjectileSprite(projectileTexture, startPosition, mousePosition, inaccuracy)
{
	speed = 1000;
	damage = 25;

}

PelletSprite::PelletSprite(sf::Texture& projectileTexture, sf::Vector2f startPosition, sf::Vector2i mousePosition, int inaccuracy)
						: ProjectileSprite(projectileTexture, startPosition, mousePosition, inaccuracy)
{
	speed = 300;
	damage = 5;
}
