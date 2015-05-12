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

	Object(sf::Texture& objectTexture);
	void run();
	void processEvent(sf::Event event);
	void render();
	void handlePlayerInput(sf::Keyboard::Key key, bool isPressed);
	void update(sf::Time TimePerFrame);
	void update(float enemyPositionX, float enemyPositionY,
	            float playerPositionX, float playerPositionY);
	void approach(float enemyPositionX, float enemyPositionY,
	              float playerPositionX, float playerPositionY);
	int getHitpoints() const;
	void setHitpoints(int hp);

	sf::Sprite sprite;
	sf::Sprite *spriteCursor;
	sf::Texture *ObjectTex;
	sf::Texture *textureCursor;
	sf::View fixed;
	sf::Vector2i mouse;
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