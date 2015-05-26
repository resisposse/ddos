#ifndef STATEMANAGER
#define STATEMANAGER

#include <stack>
#include <SFML/Graphics.hpp>

class GameState;

class StateManager
{
public:
	std::stack<GameState*> states;

	StateManager();
	~StateManager();
	void pushState(GameState* state);
	void popState();
	void changeState(GameState* state);
	GameState* peekState();

	void gameLoop();
};

#endif