/*
 * Dark Domains Of Space
 * 2015 © Project Team (see: LICENSE)
 */

#ifndef OBJECT
#define OBJECT

class Weapon;

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
	float clock;
	bool isPlaying = false;
	int random;
	bool newPath = false;
	float pathCooldown = 0;
	sf::Sprite sprite;
	sf::View fixed;
	sf::Vector2i mouse;
	std::vector<sf::Vector2f> playerPath;
	std::vector<sf::Vector2f> newPlayerPath;
	// sf::Texture *ObjectTex;
	Object(sf::Texture& objectTexture);
	void render();
	virtual void update(float frameClock);
	virtual void update(float enemyPositionX, float enemyPositionY,
		float playerPositionX, float playerPositionY) {}
	virtual void approach(float enemyPositionX, float enemyPositionY,
		float playerPositionX, float playerPositionY){}
	void approachPath(float enemyPositionX, float enemyPositionY, float realPlayerPositionX, float realPlayerPositionY);
	int lineOfSight(int startX, int startY, int endX, int endY);
	std::vector<sf::Vector2f> getLine(int startX, int startY, int endX, int endY);
	int getDistanceBetweenTiles(int x0, int y0, int x1, int y1);
	void enemyShoot(sf::Vector2i coords, float distanceX, float distanceY);
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
	void updateAggro(float amount);
	void setValue(int givenValue);
	int getValue() const;
	float getEnemySpeed() const;
	void setEnemySpeed(float amount);
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
	float enemySpeed;
};

class Player : public Object
{
public:
	std::vector<Weapon*> weapons;
	int heldWeapon = 0;

	Player(sf::Texture &objectTexture, sf::Texture &weaponTexture, sf::Vector2f coords);
	void update(float frameClock);
	void shoot();
};

class EnemyMelee : public Object
{
public:
	EnemyMelee(sf::Texture& objectTexture, sf::Vector2f coords);
	void update(float enemyPositionX, float enemyPositionY,
		float playerPositionX, float playerPositionY);
	void approach(float enemyPositionX, float enemyPositionY,
		float playerPositionX, float playerPositionY);
};

class EnemySoldier : public Object
{
public:
	EnemySoldier(sf::Texture& objectTexture, sf::Vector2f coords);
	void update(float enemyPositionX, float enemyPositionY,
		float playerPositionX, float playerPositionY);
	void approach(float enemyPositionX, float enemyPositionY,
		float playerPositionX, float playerPositionY);
	int approachTiles;
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

class BloodSmall : public Object
{
public:
	BloodSmall(sf::Texture& objectTexture, sf::Vector2f coords);
};

class BloodMedium : public Object
{
public:
	BloodMedium(sf::Texture& objectTexture, sf::Vector2f coords);
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
