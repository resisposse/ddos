/*
 * Dark Domains Of Space
 * 2015 © Project Team (see: LICENSE)
 */

#ifndef OBJECT
#define OBJECT

class Object
{
public:
	Object();
	~Object();
	void run();
	void processEvent(sf::Event event);
	void update(sf::Time TimePerFrame);
	void render();
	void handlePlayerInput(sf::Keyboard::Key key, bool isPressed);

	float updateCollisionX(float X, float Y);
	float updateCollisionY();
	static const float PlayerSpeed;
	static const sf::Time TimePerFrame;
	sf::View fixed;
	sf::Texture *ObjectTex;
	sf::Texture *TextureCursor;
	sf::Sprite *spriteCursor;

	bool mIsMovingUp;
	bool mIsMovingDown;
	bool mIsMovingRight;
	bool mIsMovingLeft;
	float objectX[3];
	float objectY[3];
	sf::Vector2i mouse;

	void approach(float positionEnemyX, float positionEnemyY, float positionPlayerX, float positionPlayerY);

	explicit Object(int hitpoints);
	void repair(int points);
	void damage(int points);
	void destroy();

	int getHitpoints() const;
	void setHitpoints(int hp);
	bool isDestroyed() const;
private:
	int mHitpoints;
};
extern sf::Sprite *mPlayerSpr;
extern sf::Sprite *mEnemySpr;

struct AircraftData
{
	int hitpoints;
	float speed;
};

struct Direction
{
	Direction(float angle, float distance);

	float angle;
	float distance;
};

#endif

class Player : public Object {
public:
	Player();
	~Player();
};

class EnemyMelee : public Object {
public:
	EnemyMelee();
	~EnemyMelee();
};
