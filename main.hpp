/*
 * Project Name
 * 2015 � Project Team (see: LICENSE)
 */

#ifndef MAIN
#define MAIN

/* Forward declarations due to cyclic dependencies */
class Object;
struct Map;

struct Game
{
	long currentClock = 0;
	long lastClock = 0;
	bool running;
	bool focused;
	float zoomLevel;
	Map *map;
	Object *object;
	Game();
	~Game();
	int collision(float x, float y);
	void update();
	void render();
	void processEvents();
	void processEvent(sf::Event event);
	void addSource();
};
extern float frameClock;
extern float lastClockTmp;
extern sf::RenderWindow *app;
extern sf::View *playerView;
extern sf::View *backgroundView;
extern Game *game;
extern sf::Clock timer;

#endif
