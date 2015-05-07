/*
 * Dark Domains Of Space
 * 2015 © Project Team (see: LICENSE)
 */

#ifndef OBJECT
#define OBJECT

class Object
{
public:
	static const float PlayerSpeed;
	static const sf::Time TimePerFrame;
	bool mIsMovingUp;
	bool mIsMovingDown;
	bool mIsMovingRight;
	bool mIsMovingLeft;

	Object();
	void run();
	void processEvent(sf::Event event);
	void render();
	void handlePlayerInput(sf::Keyboard::Key key, bool isPressed);
	void update(sf::Time TimePerFrame);
	void update(float enemyPositionX, float enemyPositionY,
	            float playerPositionX, float playerPositionY);
	void approach(float enemyPositionX, float enemyPositionY,
	              float playerPositionX, float playerPositionY);

	void repair(int points);
	void damage(int points);
	void destroy();
	int getHitpoints() const;
	void setHitpoints(int hp);
	bool isDestroyed() const;

	sf::Sprite sprite;
	sf::Sprite *spriteCursor;
	sf::Texture *ObjectTex;
	sf::Texture *TextureCursor;
	sf::View fixed;
	sf::Vector2i mouse;
private:
	int mHitpoints;
};

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

class Player : public Object {
public:
	Player();
};

class EnemyMelee : public Object {
public:
	EnemyMelee();
};

#endif
