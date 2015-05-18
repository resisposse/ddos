/*
 * Dark Domains Of Space
 * 2015 © Project Team (see: LICENSE)
 */

#ifndef OBJECT
#define OBJECT

class Object
{
public:
	bool mIsMovingUp;
	bool mIsMovingDown;
	bool mIsMovingRight;
	bool mIsMovingLeft;
	bool playerShooting;
	float playerSpeed;
	float shootingCooldown = 0;
	sf::Sprite sprite;
	sf::View fixed;
	sf::Vector2i mouse;
	// sf::Texture *ObjectTex;

	Object(sf::Texture& objectTexture);
	void render();
	void update(float frameClock);
	void update(float enemyPositionX, float enemyPositionY,
	            float playerPositionX, float playerPositionY);
	void approach(float enemyPositionX, float enemyPositionY,
	              float playerPositionX, float playerPositionY);
	void playerShoot();
	void enemyShoot(sf::Vector2i coords);
	int getHitpoints() const;
	void setHitpoints(int hp);
	float getMeleeDamage() const;
	void setMeleeDamage(float damage);
	void setDamage(float damage);
	bool isDestroyed() const;
	void setCooldown(float amount);
	void setAggro(float amount);
	float getAggro() const;
private:
	int mHitpoints;
	float meleeDamage;
	float cooldown;
	float aggro;
};

class Player : public Object
{
public:
	Player(sf::Texture& objectTexture, sf::Vector2f coords);
};

class EnemyMelee : public Object
{
public:
	EnemyMelee(sf::Texture& objectTexture, sf::Vector2f coords);
};

class EnemySoldier : public Object
{
public:
	EnemySoldier(sf::Texture& objectTexture, sf::Vector2f coords);
};

class HealthBar : public Object
{
public:
	HealthBar(sf::Texture& objectTexture);
};

#endif
