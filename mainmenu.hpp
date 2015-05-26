#ifndef MAINMENU
#define MAINMENU

#include <SFML/Graphics.hpp>
#include "menuitem.hpp"
#include "gamestate.hpp"

class MainMenu : public GameState
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

	MainMenu(StateManager *stateManager);

	std::vector<MenuItem> menuItems;
	MenuItem title;
};

#endif