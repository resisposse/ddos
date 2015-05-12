/*
 * Dark Domains Of Space
 * 2015 © Project Team (see: LICENSE)
 */

#ifndef PROJECTILE
#define PROJECTILE

class ProjectileSprite
{
public:
	sf::Sprite sprite;
	sf::Vector2f position;
	sf::Vector2f normalized;
	int speed;
	int damage;
	float xVelocity;
	float yVelocity;
	float inaccuracyAmount = 0.f;

	ProjectileSprite(sf::Texture& projectileTexture, sf::Vector2f startPosition, sf::Vector2i mousePosition, int inaccuracy);
	void update(float second);
	//bool outsideWindow();
};

class BulletSprite: public ProjectileSprite
{
public:
	BulletSprite(sf::Texture& projectileTexture, sf::Vector2f startPosition, sf::Vector2i mousePosition, int inaccuracy);
};

class LaserSprite : public ProjectileSprite
{
public:
	LaserSprite(sf::Texture& projectileTexture, sf::Vector2f startPosition, sf::Vector2i mousePosition, int inaccuracy);
};

class PelletSprite : public ProjectileSprite
{
public:
	PelletSprite(sf::Texture& projectileTexture, sf::Vector2f startPosition, sf::Vector2i mousePosition, int inaccuracy);
};
#endif
