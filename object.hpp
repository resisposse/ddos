/*
 * Dark Domains Of Space
 * 2015 © Project Team (see: LICENSE)
 */

#ifndef OBJECT
#define OBJECT

class Object
{
public:
	Object(sf::Texture& objectTexture);
	void run();
	void processEvent(sf::Event event);
	void update(sf::Time TimePerFrame, float enemyPosX, float enemyPosY, float playerPosX, float playerPosY);
	void update(sf::Time TimePerFrame);
	void render();
	void handlePlayerInput(sf::Keyboard::Key key, bool isPressed);

	sf::Sprite sprite;

	float updateCollisionX(float X, float Y);
	float updateCollisionY();
	static const float PlayerSpeed;
	static const sf::Time TimePerFrame;
	sf::View fixed;

	bool mIsMovingUp;
	bool mIsMovingDown;
	bool mIsMovingRight;
	bool mIsMovingLeft;
	float objectX[3];
	float objectY[3];
	sf::Vector2i mouse;

	void approach(float positionEnemyX, float positionEnemyY, float positionPlayerX, float positionPlayerY);

	explicit Object(int hitpoints);
	//void repair(int points);
	void damage(int points);
	//void destroy();

	int getHitpoints() const;
	void setHitpoints(int hp);
	//bool isDestroyed() const;
private:
	int mHitpoints;
};

class Player : public Object {
public:
	Player(sf::Texture& objectTexture);
};

class EnemyMelee : public Object {
public:
	EnemyMelee(sf::Texture& objectTexture);
};

#endif