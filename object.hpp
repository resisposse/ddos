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
	static const float PlayerSpeed;
	static const sf::Time TimePerFrame;
	sf::Texture *mTexture;
	bool mIsMovingUp;
	bool mIsMovingDown;
	bool mIsMovingRight;
	bool mIsMovingLeft;
	sf::Vector2i mouse;
};
extern sf::Sprite *mPlayerSpr;
#endif
