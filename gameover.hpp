#ifndef GAMEOVER
#define GAMEOVER

#include <SFML/Graphics.hpp>
#include "menuitem.hpp"
#include "gamestate.hpp"

class GameOver : public GameState
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

	GameOver(StateManager *stateManager);

	std::vector<MenuItem> menuButtons;
	MenuItem gameOverText;
	MenuItem deathText;
	MenuItem score;
};

#endif