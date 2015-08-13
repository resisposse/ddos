/*
 * Dark Domains Of Space
 * 2015 © Project Team (see: LICENSE)
 */

#ifndef OBJECT
#define OBJECT

class Weapon;
class Map;

/*
 * ScoreValue
 *
 * - Object
 *   - Character
 *     - Enemy << ScoreValue
 *       - EnemyMelee
 *       - EnemySoldier
 *     - Player
 *   - Item
 *     - Valuable << ScoreValue
 *       - ValuableLow
 *       - ValuableMid
 *       - ValuableHigh
 *   - Blood
 *     - BloodSmall
 *     - BloodMedium
 *   - HealthBar
 *   - ShieldBar
 */

class ScoreValue
{
public:
	void setValue(int givenValue);
	int getValue() const;
protected:
	int value;
};


/*
 * + Object
 */

class Object
{
public:
	sf::Sprite sprite;
	sf::View fixed;
	sf::Vector2i mouse;
	// sf::Texture *ObjectTex;
	Object(sf::Texture& objectTexture);
	void render();
	virtual void update() {};
	bool isDestroyed() const;
};

/*
* --------------
* - Object
*   + Character <-
*   + Item
*   + Blood
*   - HealthBar
* --------------
*/

class Character : public Object
{
public:
	float maxShieldPoints = 0.0;
	float shieldRechargeDelay = 0.0;
	float shieldTimeUntilRecharge = 0.0;
	bool isPlaying = false;
	float shootingCooldown = 0;

	Character(sf::Texture& objectTexture) : Object(objectTexture) {};
	void takeDamage(int damage);
	int getHitpoints() const;
	void setHitpoints(int hp);
	float getShieldpoints() const;
	void setShieldpoints(float shield);
	void updateShield(float frameClock);
	bool setDamage(float damage);
	float getMeleeDamage() const;
	void setMeleeDamage(float damage);
private:
	int mHitpoints;
	float mShieldpoints;
	float meleeDamage;
};

/*
* ----------------------------
* - Object
*   - Character
*     + Enemy << ScoreValue <-
*     - Player
*   + Item
* ----------------------------
*/

class Enemy : public Character, public ScoreValue
{
public:
	float clock;
	int random;
	std::vector<sf::Vector2f> playerPath;
	std::vector<sf::Vector2f> newPlayerPath;
	bool newPath = false;
	float pathCooldown = 0;

	Enemy(sf::Texture& objectTexture) : Character(objectTexture) {};
	virtual void update(Map *map, float enemyPositionX, float enemyPositionY,
		float playerPositionX, float playerPositionY) = 0;
	virtual void approach(Map *map, float enemyPositionX, float enemyPositionY,
		float playerPositionX, float playerPositionY) = 0;
	void approachPath(float enemyPositionX, float enemyPositionY, float realPlayerPositionX, float realPlayerPositionY);
	int lineOfSight(int startX, int startY, int endX, int endY);
	std::vector<sf::Vector2f> getLine(int startX, int startY, int endX, int endY);
	int getDistanceBetweenTiles(int x0, int y0, int x1, int y1);

	float getEnemySpeed() const;
	void setEnemySpeed(float amount);
	void setCooldown(float amount);
	void setAggro(float amount);
	float getAggro() const;
	void updateAggro(float amount);
protected:
	float cooldown;
	float aggro = 0;
	float enemySpeed;
};

/*
* ----------------------------
*   - Character
*     - Enemy << ScoreValue
*       - EnemyMelee        <-
*       - EnemySoldier
*     + Player
* ----------------------------
*/

class EnemyMelee : public Enemy
{
public:
	EnemyMelee(sf::Texture& objectTexture, sf::Vector2f coords);
	void update(Map *map, float enemyPositionX, float enemyPositionY,
		float playerPositionX, float playerPositionY);
	void approach(Map *map, float enemyPositionX, float enemyPositionY,
		float playerPositionX, float playerPositionY);
};

/*
* ----------------------------
*     - Enemy <- ScoreValue
*       - EnemyMelee
*       - EnemySoldier      <-
*     - Player
*   - Item
* ----------------------------
*/

class EnemySoldier : public Enemy
{
public:
	int approachTiles;

	EnemySoldier(sf::Texture& objectTexture, sf::Vector2f coords);
	virtual void update(Map *map, float enemyPositionX, float enemyPositionY,
		float playerPositionX, float playerPositionY);
	virtual void approach(Map *map, float enemyPositionX, float enemyPositionY,
		float playerPositionX, float playerPositionY);
	void shoot(sf::Vector2i coords, float distanceX, float distanceY);
};


/*
* ---------------------------
*   - Character
*     + Enemy << ScoreValue
*     - Player              <-
*   + Item
*   + Blood
* ----------------------------
*/

class Player : public Character
{
public:
	bool mIsMovingUp;
	bool mIsMovingDown;
	bool mIsMovingRight;
	bool mIsMovingLeft;
	bool playerShooting;
	float playerSpeed;
	std::vector<Weapon*> weapons;
	int heldWeapon = 0;

	Player(sf::Texture &objectTexture, sf::Texture &weaponTexture, sf::Vector2f coords);
	void update(Map *map, float frameClock);
	void shoot();
};

/*
* ----------------
* - Object
*   - Character
*   - Item      <-
*   - Blood
*   - HealthBar
* ----------------
*/

class Item : public Object
{
public:
	Item(sf::Texture& objectTexture) : Object(objectTexture) {};
};

/*
* -------------------------------
*   - Character
*   - Item
*     + Valuable << ScoreValue <-
*   - Blood
*   - HealthBar
* -------------------------------
*/

class Valuable : public Item, public ScoreValue
{
public:
	Valuable(sf::Texture& objectTexture) : Item(objectTexture) {};
};

/*
* -------------------------------
*   - Item
*     - Valuable << ScoreValue
*       - ValuableLow          <-
*       - ValuableMid
*       - ValuableHigh
* -------------------------------
*/

class ValuableLow : public Valuable
{
public:
	ValuableLow(sf::Texture& objectTexture, sf::Vector2f coords);
};

class ValuableMed : public Valuable
{
public:
	ValuableMed(sf::Texture& objectTexture, sf::Vector2f coords);
};

class ValuableHigh : public Valuable
{
public:
	ValuableHigh(sf::Texture& objectTexture, sf::Vector2f coords);
};

/*
* --------------------
*   - Character
*   + Item
*   - Blood         <-
*     - BloodSmall
*     - BloodMedium
* --------------------
*/

class Blood : public Object
{
public:
	Blood(sf::Texture& objectTexture) : Object(objectTexture) {};
};

class BloodSmall : public Blood
{
public:
	BloodSmall(sf::Texture& objectTexture, sf::Vector2f coords);
};

class BloodMedium : public Blood
{
public:
	BloodMedium(sf::Texture& objectTexture, sf::Vector2f coords);
};

/*
* ----------------
*   + Item
*   + Blood
*   - HealthBar <-
*   - ShieldBar
*/

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
