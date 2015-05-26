#include <stack>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "globals.hpp"
#include "gamestate.hpp"
#include "statemanager.hpp"

sf::RenderWindow *app;

StateManager::StateManager()
{
	app = new sf::RenderWindow(sf::VideoMode(800, 608, 32),
		"Dark Domains Of Space",
		sf::Style::Resize | sf::Style::Close);
	app->setFramerateLimit(60);
}

StateManager::~StateManager()
{
	while (!this->states.empty()) popState();
	delete app;
}

void StateManager::pushState(GameState* state)
{
	this->states.push(state);
	return;
}

void StateManager::popState()
{
	delete this->states.top();
	this->states.pop();

	return;
}

void StateManager::changeState(GameState* state)
{
	if (!this->states.empty())
		popState();
	pushState(state);

	return;
}

GameState* StateManager::peekState()
{
	if (this->states.empty()) {
		return nullptr;
	}
	return this->states.top();
}

void StateManager::gameLoop()
{
	sf::Clock clock;

	while (app->isOpen()) {
		if (peekState() == nullptr) {
			continue;
		}

		peekState()->handleInput();
		peekState()->update();
		app->clear(sf::Color::Black);
		peekState()->draw();
		app->display();
	}
}