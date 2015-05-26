#ifndef PAUSE
#define PAUSE

#include <SFML/Graphics.hpp>
#include "menuitem.hpp"
#include "gamestate.hpp"

class Pause : public GameState
{
private:
	sf::View view;
	sf::Texture menuBgTex;
	sf::Sprite menuBgSpr;
	sf::Font *font;
public:
	virtual void draw();
	virtual void update();
	virtual void handleInput();

	Pause(StateManager *stateManager);

	std::vector<MenuItem> menuButtons;
	MenuItem gameOverText;
	MenuItem deathText;
	MenuItem score;
};

#endif