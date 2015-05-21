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
	void updateShield(float frameClock);
	int getHitpoints() const;
	void setHitpoints(int hp);
	float getShieldpoints() const;
	void setShieldpoints(float shield);
	float getMeleeDamage() const;
	void setMeleeDamage(float damage);
	void setDamage(float damage);
	bool isDestroyed() const;
	void setCooldown(float amount);
	void setAggro(float amount);
	float getAggro() const;
	void setValue(int givenValue);
	int getValue() const;
	float maxShieldPoints;
	float shieldRechargeDelay;
	float shieldTimeUntilRecharge = 0.0;
private:
	int mHitpoints;
	float mShieldpoints;
	float meleeDamage;
	float cooldown;
	float aggro = 0;
	int value;
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

class ValuableLow : public Object
{
public:
	ValuableLow(sf::Texture& objectTexture, sf::Vector2f coords);
};

class ValuableMed : public Object
{
public:
	ValuableMed(sf::Texture& objectTexture, sf::Vector2f coords);
};

class ValuableHigh : public Object
{
public:
	ValuableHigh(sf::Texture& objectTexture, sf::Vector2f coords);
};

class HealthBar : public Object
{
public:
	HealthBar(sf::Texture& objectTexture);
};

class ShieldBar : public Object
{
public:
	ShieldBar(sf::Texture& objectTexture);
};

#endif
