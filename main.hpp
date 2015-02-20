/*
* Project Name
* 2015 © Project Team (see: LICENSE)
*/

struct Game
{
	sf::Clock *clock;
	long currentClock;
	long lastClock;
	bool running;
	bool focused;
	GameState state;
	Map *map;
	Game();
	~Game();
	void update();
	void render();
	void stop() { running = false; };
	void processEvents();
	void processEvent(sf::Event event);
	void addSource();
};
extern float frameClock;
extern sf::RenderWindow *app;
extern Game *game;
