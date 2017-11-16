#ifndef MENU
#define MENU

#include <SFML/Graphics.hpp>
#include "gamestate.hpp"

class MenuItem
{
public:
	sf::Rect<int> hitBox;
	sf::RectangleShape rect;
	sf::Text text;
	sf::Color fontColor;
	sf::Color fontHighlightColor;

	MenuItem(int x, int y, int width, int height, sf::Color boxColor, sf::Color fontColor, sf::Color fontHighlightColor, std::string string, sf::Font &font, int fontSize = 30)
	{
		this->fontColor = fontColor;
		this->fontHighlightColor = fontHighlightColor;

		hitBox.top = y - height / 2;
		hitBox.left = x - width / 2;
		hitBox.height = height;
		hitBox.width = width;
		rect.setFillColor(boxColor);
		rect.setOrigin(width / 2, height / 2);
		rect.setPosition(sf::Vector2f(x, y));
		rect.setSize(sf::Vector2f(width, height));
		text.setString(string);
		text.setFont(font);
		text.setColor(this->fontColor);
		text.setCharacterSize(fontSize);
		sf::FloatRect textRect = text.getLocalBounds();
		text.setOrigin(textRect.left + textRect.width / 2.0f,
			textRect.top + textRect.height / 2.0f);
		text.setPosition(sf::Vector2f(x, y));
	};
	MenuItem() {};
};

class Menu : public GameState
{
public:
	virtual void draw();
	virtual void update();
protected:
	sf::View view;
	sf::Texture menuBgTex;
	sf::Sprite menuBgSpr;
	sf::Font font;
	std::vector<MenuItem> menuItems;
};

class MainMenu : public Menu
{
public:
	virtual void handleInput();
	MainMenu(StateManager *stateManager);
	~MainMenu();
};


class Pause : public Menu
{
public:
	virtual void handleInput();
	Pause(StateManager *stateManager, sf::String score);
	~Pause();
};

class GameOver : public Menu
{
public:
	virtual void handleInput();
	GameOver(StateManager *stateManager, sf::String score);
	~GameOver();
};

#endif
