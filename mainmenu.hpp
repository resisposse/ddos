#ifndef MAINMENU
#define MAINMENU

#include <SFML/Graphics.hpp>
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
	class MenuItem
	{
	public:
		sf::Rect<int> hitBox;
		sf::RectangleShape rect;
		sf::Text text;

		MenuItem(int x, int y, int width, int height, sf::Color color, std::string string, sf::Font &font, int fontSize = 30)
		{
			hitBox.top = y - height / 2;
			hitBox.left = x - width / 2;
			hitBox.height = height;
			hitBox.width = width;
			rect.setFillColor(color);
			rect.setOrigin(width / 2, height / 2);
			rect.setPosition(sf::Vector2f(x, y));
			rect.setSize(sf::Vector2f(width, height));
			text.setString(string);
			text.setFont(font);
			text.setCharacterSize(fontSize);
			sf::FloatRect textRect = text.getLocalBounds();
			text.setOrigin(textRect.left + textRect.width / 2.0f,
				textRect.top + textRect.height / 2.0f);
			text.setPosition(sf::Vector2f(x, y));
		};
		MenuItem() {};
	};

	std::vector<MenuItem> menuItems;
	MenuItem title;
};

#endif