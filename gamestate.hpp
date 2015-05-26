#ifndef GAMESTATE
#define GAMESTATE

#include "statemanager.hpp"

class GameState
{
public:

	StateManager *stateManager;

	virtual void draw() = 0;
	virtual void update() = 0;
	virtual void handleInput() = 0;
};

#endif