#include <SFML/Graphics.hpp>
#include <iostream>

#include "globals.hpp"
#include "main.hpp"
#include "gamestate.hpp"
#include "gameover.hpp"

GameOver::GameOver(StateManager *stateManager)
{
	menuBgTex = sf::Texture();
	menuBgTex.loadFromFile("media/ddos-menu-bg.jpg");
	menuBgSpr = sf::Sprite(menuBgTex);
	menuBgSpr.setColor(sf::Color(50, 50, 50, 255));

	font = new sf::Font;
	font->loadFromFile("fonts/Exo-Regular.otf");

	this->stateManager = stateManager;
	sf::Vector2f pos = sf::Vector2f(app->getSize());
	this->view.setSize(pos);
	pos *= 0.5f;
	this->view.setCenter(pos);

	int x = (int)this->view.getSize().x / 2;
	int y = ((int)this->view.getSize().y / 4) * 3;
	int width = (int)this->view.getSize().x / 3;
	int height = (int)this->view.getSize().y / 20;
	int padding = (int)this->view.getSize().y / 100;

	gameOverText = MenuItem(x, y / 2, width * 2, height * 2, sf::Color::Transparent, sf::Color::Red, "You Died", *font, 60);
	score = MenuItem(x, (y / 2) + height * 2 + padding * 2, width * 2, height * 2, sf::Color::Transparent, sf::Color::Red, game->scoreText.getString(), *font, 25);

	MenuItem restartButton(x, y, width, height, sf::Color::Transparent, sf::Color::White, "Restart Game", *font);
	MenuItem returnButton(x, y + height + padding, width, height, sf::Color::Transparent, sf::Color::White, "Main Menu", *font);

	menuButtons.push_back(restartButton);
	menuButtons.push_back(returnButton);

	std::cout << menuButtons.size() << std::endl;
}

void GameOver::draw()
{
	app->setView(this->view);
	app->clear(sf::Color::Black);
	float scale = app->getSize().y / float(this->menuBgSpr.getTexture()->getSize().y);
	this->menuBgSpr.setScale(sf::Vector2f(scale, scale));
	app->draw(this->menuBgSpr);
	for (unsigned int i = 0; i < menuButtons.size(); i++) {
		app->draw(menuButtons[i].rect);
		if (menuButtons[i].hitBox.contains(sf::Vector2i(app->mapPixelToCoords(sf::Mouse::getPosition(*app))))) {
			menuButtons[i].text.setColor(sf::Color(0,70,255,255));
		} else {
			menuButtons[i].text.setColor(sf::Color::White);
		}
		app->draw(menuButtons[i].text);
	}
	app->draw(gameOverText.text);
	app->draw(score.text);
}

void GameOver::update()
{

}

void GameOver::handleInput()
{
	sf::Event event;

	while (app->pollEvent(event)) {
		switch (event.type) {
			/* Close the window */

		case sf::Event::MouseButtonPressed: {
			switch (event.mouseButton.button) {
			case sf::Mouse::Left:
				if (menuButtons[0].hitBox.contains(sf::Vector2i(app->mapPixelToCoords(sf::Mouse::getPosition(*app))))) {
					game = new Game(this->stateManager);
					stateManager->changeState(game);
				} else if (menuButtons[1].hitBox.contains(sf::Vector2i(app->mapPixelToCoords(sf::Mouse::getPosition(*app))))) {
					stateManager->popState();
				}
				break;
			}
			break;
		}
		case sf::Event::Closed:
		{
			app->close();
			break;
		}
			/* Resize the window */
		case sf::Event::Resized:
		{
			this->view.setSize(event.size.width, event.size.height);
			this->menuBgSpr.setPosition(app->mapPixelToCoords(sf::Vector2i(0, 0)));
			/*this->menuBgSpr.setScale(
			float(event.size.width) / float(this->menuBgSpr.getTexture()->getSize().x),
			float(event.size.height) / float(this->menuBgSpr.getTexture()->getSize().y));*/

			float scale = app->getSize().y / float(this->menuBgSpr.getTexture()->getSize().y);
			this->menuBgSpr.setScale(sf::Vector2f(scale, scale));
			break;
		}
		case sf::Event::KeyPressed:
		{
			if (event.key.code == sf::Keyboard::Escape) app->close();
			break;
		}
		default: break;
		}
	}
}
