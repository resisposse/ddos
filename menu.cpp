#include <SFML/Graphics.hpp>

#include "globals.hpp"
#include "main.hpp"
#include "gamestate.hpp"
#include "menu.hpp"

void Menu::draw()
{
	app->setView(this->view);
	app->clear(sf::Color::Black);
	float scale = app->getSize().y / float(this->menuBgSpr.getTexture()->getSize().y);
	this->menuBgSpr.setScale(sf::Vector2f(scale, scale));
	app->draw(this->menuBgSpr);
	for (unsigned int i = 0; i < menuItems.size(); i++) {
		app->draw(menuItems[i].rect);
		if (menuItems[i].hitBox.contains(sf::Vector2i(app->mapPixelToCoords(sf::Mouse::getPosition(*app))))) {
			menuItems[i].text.setColor(menuItems[i].fontHighlightColor);
		} else {
			menuItems[i].text.setColor(menuItems[i].fontColor);
		}
		app->draw(menuItems[i].text);
	}
}

void Menu::update()
{

}

MainMenu::MainMenu(StateManager *stateManager)
{
	menuBgTex = sf::Texture();
	menuBgTex.loadFromFile("media/ddos-menu-bg.jpg");
	menuBgSpr = sf::Sprite(menuBgTex);

	font.loadFromFile("fonts/Exo-Regular.otf");

	this->stateManager = stateManager;
	sf::Vector2f pos = sf::Vector2f(app->getSize());
	this->view.setSize(pos);
	pos *= 0.5f;
	this->view.setCenter(pos);

	int x = (int)this->view.getSize().x / 2;
	int y = (int)this->view.getSize().y / 2;
	int width = (int)this->view.getSize().x / 3;
	int height = (int)this->view.getSize().y / 20;
	int padding = (int)this->view.getSize().y / 100;

	MenuItem title(x, y / 2, width * 2, height * 2, sf::Color::Transparent, sf::Color::White, sf::Color::White, "Dark Domains of Space", font, 60);
	MenuItem playButton(x, y, width, height, sf::Color::Transparent, sf::Color::White, sf::Color(0, 70, 255, 255), "Play Game", font);
	MenuItem exitButton(x, y + height + padding, width, height, sf::Color::Transparent, sf::Color::White, sf::Color(0, 70, 255, 255), "Exit Game", font);

	menuItems.push_back(playButton);
	menuItems.push_back(exitButton);
	menuItems.push_back(title);
}

void MainMenu::handleInput()
{
	sf::Event event;

	while (app->pollEvent(event)) {
		switch (event.type) {
			/* Close the window */

		case sf::Event::MouseButtonPressed: {
			switch (event.mouseButton.button) {
			case sf::Mouse::Left:
				if (menuItems[0].hitBox.contains(sf::Vector2i(app->mapPixelToCoords(sf::Mouse::getPosition(*app))))) {
					game = new Game(this->stateManager);
					stateManager->pushState(game);
				} else if (menuItems[1].hitBox.contains(sf::Vector2i(app->mapPixelToCoords(sf::Mouse::getPosition(*app))))) {
					app->close();
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

Pause::Pause(StateManager *stateManager, sf::String score)
{
	menuBgTex = sf::Texture();
	menuBgTex.loadFromFile("media/ddos-menu-bg.jpg");
	menuBgSpr = sf::Sprite(menuBgTex);
	menuBgSpr.setColor(sf::Color(50, 50, 50, 255));

	font.loadFromFile("fonts/Exo-Regular.otf");

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

	MenuItem restartButton(x, y, width, height, sf::Color::Transparent, sf::Color::White, sf::Color(0, 70, 255, 255), "Return Game", font);
	MenuItem returnButton(x, y + height + padding, width, height, sf::Color::Transparent, sf::Color::White, sf::Color(0, 70, 255, 255), "Main Menu", font);
	MenuItem gameOverText(x, y / 2, width * 2, height * 2, sf::Color::Transparent, sf::Color(0, 170, 255, 255), sf::Color(0, 170, 255, 255), "Paused", font, 60);
	MenuItem scoreText(x, (y / 2) + height * 2 + padding * 2, width * 2, height * 2, sf::Color::Transparent, sf::Color::White, sf::Color::White, score, font, 25);

	menuItems.push_back(restartButton);
	menuItems.push_back(returnButton);
	menuItems.push_back(gameOverText);
	menuItems.push_back(scoreText);
}

void Pause::handleInput()
{
	sf::Event event;

	while (app->pollEvent(event)) {
		switch (event.type) {
			/* Close the window */

		case sf::Event::MouseButtonPressed: {
			switch (event.mouseButton.button) {
			case sf::Mouse::Left:
				if (menuItems[0].hitBox.contains(sf::Vector2i(app->mapPixelToCoords(sf::Mouse::getPosition(*app))))) {
					app->setMouseCursorVisible(false);
					stateManager->unSuspendState();
				} else if (menuItems[1].hitBox.contains(sf::Vector2i(app->mapPixelToCoords(sf::Mouse::getPosition(*app))))) {
					stateManager->clearSuspendedState();
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
			if (event.key.code == sf::Keyboard::Escape) {
				app->setMouseCursorVisible(false);
				stateManager->unSuspendState();
			}
			break;
		}
		default: break;
		}
	}
}

GameOver::GameOver(StateManager *stateManager, sf::String score)
{
	menuBgTex = sf::Texture();
	menuBgTex.loadFromFile("media/ddos-menu-bg.jpg");
	menuBgSpr = sf::Sprite(menuBgTex);
	menuBgSpr.setColor(sf::Color(50, 50, 50, 255));

	font.loadFromFile("fonts/Exo-Regular.otf");

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

	MenuItem restartButton(x, y, width, height, sf::Color::Transparent, sf::Color::White, sf::Color(0, 70, 255, 255), "Restart Game", font);
	MenuItem returnButton(x, y + height + padding, width, height, sf::Color::Transparent, sf::Color::White, sf::Color(0, 70, 255, 255), "Main Menu", font);

	MenuItem gameOverText(x, y / 2, width * 2, height * 2, sf::Color::Transparent, sf::Color::Red, sf::Color::Red, "You Died", font, 60);
	MenuItem scoreText(x, (y / 2) + height * 2 + padding * 2, width * 2, height * 2, sf::Color::Transparent, sf::Color::Red, sf::Color::Red, score, font, 25);

	menuItems.push_back(restartButton);
	menuItems.push_back(returnButton);

	menuItems.push_back(gameOverText);
	menuItems.push_back(scoreText);
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
				if (menuItems[0].hitBox.contains(sf::Vector2i(app->mapPixelToCoords(sf::Mouse::getPosition(*app))))) {
					game = new Game(this->stateManager);
					stateManager->changeState(game);
				} else if (menuItems[1].hitBox.contains(sf::Vector2i(app->mapPixelToCoords(sf::Mouse::getPosition(*app))))) {
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
