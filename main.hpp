/*
* Project Name
* 2015 © Project Team (see: LICENSE)
*/

#ifndef MAIN
#define MAIN

/* Forward declarations due to cyclic dependencies */
class Object;
struct Map;

struct Game
{
	long currentClock;
	long lastClock;
	bool running;
	bool focused;
	Map *map;
	Object *object;
	Game();
	~Game();
	void update();
	void render();
	void processEvents();
	void processEvent(sf::Event event);
	void addSource();
};
extern float frameClock;
extern sf::RenderWindow *app;
extern Game *game;
extern sf::Clock timer;

#endif
