/*
 * Dark Domains Of Space
 * 2015 © Project Team (see: LICENSE)
 */

#ifndef PROJECTILE
#define PROJECTILE

#include <cmath>
#include <SFML/Graphics.hpp>

class ProjectileSprite
{
public:
	sf::Sprite sprite;
	sf::Vector2f position;
	sf::Vector2f normalized;
	int speed;
	float xVelocity;
	float yVelocity;

	ProjectileSprite(sf::Texture& projectileTexture, sf::Vector2f startPosition, sf::Vector2i mousePosition);
	void update(float second);
	//bool outsideWindow();
};

class BulletSprite: public ProjectileSprite
{
public:
	BulletSprite(sf::Texture& projectileTexture, sf::Vector2f startPosition, sf::Vector2i mousePosition);
};

class LaserSprite : public ProjectileSprite
{
public:
	LaserSprite(sf::Texture& projectileTexture, sf::Vector2f startPosition, sf::Vector2i mousePosition);
};

#endif
