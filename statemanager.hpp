#ifndef STATEMANAGER
#define STATEMANAGER

#include <stack>
#include <SFML/Graphics.hpp>

class GameState;

class StateManager
{
public:
	std::stack<GameState*> states;
	GameState *suspendedState;

	sf::RenderWindow window;

	StateManager();
	~StateManager();
	void pushState(GameState* state);
	void popState();
	void changeState(GameState* state);
	GameState* peekState();
	void suspendState(GameState* state);
	void unSuspendState();
	void clearSuspendedState();

	void gameLoop();
};

#endif