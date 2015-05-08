/*
 * Dark Domains Of Space
 * 2015 © Project Team (see: LICENSE)
 */

#ifndef MAIN
#define MAIN

#include "mapgenerator.hpp"
#include "projectile.hpp"

/* Forward declarations due to cyclic dependencies */
class Player;
class EnemyMelee;
struct Map;

class Game
{
public:
	long currentClock = 0;
	long lastClock = 0;
	bool running;
	bool focused;
	float zoomLevel;
	float playerPositionX;
	float playerPositionY;
	int ammoType = 0;

	MapGenerator *mapGenerator;
	Map *map;
	Player *player;
	EnemyMelee *enemy;

	Game();
	~Game();
	int collision(float x, float y, std::string collisionType);
	void update();
	void render();
	void parseEvents();
	void processEvent(sf::Event event);
	void addSource();
	void loadCharacterTextures();
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
	void initializeView();
	void initializeLighting();
	void updateLighting();

	sf::Texture *playerTexture;
	sf::Texture *enemyMeleeTexture;
	sf::Texture *bulletTexture;
	sf::Texture *laserBeamTexture;
	sf::Texture *TextureCursor;
	sf::Sprite *spriteCursor;
	sf::View *playerView;
	sf::View *backgroundView;
	sf::View fixed;
	sf::Vector2i mouse;

	std::vector<ProjectileSprite> projectiles;
	std::vector<EnemyMelee> enemies;

	/* Temporary time fix, check this from main.cpp */
	static const sf::Time TimePerFrameTmp;
};
extern float frameClock;
extern float lastClockTmp;
extern sf::RenderWindow *app;
extern Game *game;
extern sf::Clock timer;

#endif
