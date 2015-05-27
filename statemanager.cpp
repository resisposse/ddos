#include <stack>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "globals.hpp"
#include "gamestate.hpp"
#include "statemanager.hpp"

sf::RenderWindow *app;

StateManager::StateManager()
{
	this->window.create(sf::VideoMode(800, 608, 32),
		"Dark Domains Of Space",
		sf::Style::Resize | sf::Style::Close);
	this->window.setFramerateLimit(60);
	app = &window;
}

StateManager::~StateManager()
{
	clearSuspendedState();
	while (!this->states.empty()) popState();
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

void StateManager::suspendState(GameState* state)
{
	this->suspendedState = this->peekState();
	if (!this->states.empty())
		this->states.pop();
	pushState(state);
	return;
}

void StateManager::unSuspendState()
{
	if (!this->states.empty())
		popState();
	this->states.push(this->suspendedState);
	this->suspendedState = nullptr;
}

void StateManager::clearSuspendedState()
{
	if (suspendedState) {
		delete suspendedState;
	}
	this->suspendedState = nullptr;
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
		window.clear(sf::Color::Black);
		peekState()->draw();
		window.display();
	}
}