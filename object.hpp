/*
 * Project Name
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
	void updatePosition(float X, float Y);
	float updateCollisionX(float X, float Y);
	float updateCollisionY();
	static const float PlayerSpeed;
	static const sf::Time TimePerFrame;
	sf::Texture *mTexture;
	bool mIsMovingUp;
	bool mIsMovingDown;
	bool mIsMovingRight;
	bool mIsMovingLeft;
	float objectX[3];
	float objectY[3];
	sf::Vector2i mouse;
};
extern sf::Sprite *mPlayerSpr;
#endif
