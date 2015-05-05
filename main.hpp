/*
 * Project Name
 * 2015 © Project Team (see: LICENSE)
 */

#ifndef MAIN
#define MAIN
#include "mapgenerator.hpp"

#include "projectileSprite.hpp"

/* Forward declarations due to cyclic dependencies */
class Player;
class EnemyMelee;
struct Map;

struct Game
{
	long currentClock = 0;
	long lastClock = 0;
	bool running;
	bool focused;
	float zoomLevel;
	int ammoType = 0;
	MapGenerator *mapGenerator;
	Map *map;
	Player *player;
	EnemyMelee *enemy;
	Game();
	sf::Texture *Texture4;
	~Game();

	sf::Vector2i mouse;
	sf::View fixed;
	sf::Texture *TextureCursor;
	sf::Sprite *spriteCursor;

	int collision(float x, float y, std::string collisionType);
	void update();
	void render();
	void processEvents();
	void processEvent(sf::Event event);
	void addSource();
	void loadCursorTexture();
	void loadProjectileTextures();
	void updateProjectiles();
	void checkProjectileCollisions();
	void drawProjectiles();
	void spawnEnemies(int amount);
	void updateEnemies();
	void drawEnemies();
	void drawPlayer();
	void drawCursor();
	sf::Texture *bulletTexture;
	sf::Texture *laserBeamTexture;
	std::vector<ProjectileSprite> projectiles;
	std::vector<EnemyMelee> enemies;

	/* Temporary time fix, check this from main.cpp */
	static const sf::Time TimePerFrameTmp;
};
extern float frameClock;
extern float lastClockTmp;
extern sf::RenderWindow *app;
extern sf::View *playerView;
extern sf::View *backgroundView;
extern Game *game;
extern sf::Clock timer;

#endif
