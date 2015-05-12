/*
 * Dark Domains Of Space
 * 2015 © Project Team (see: LICENSE)
 */

#ifndef MAIN
#define MAIN

#include "mapgenerator.hpp"
#include "projectile.hpp"
#include "weapon.hpp"

/* Forward declarations due to cyclic dependencies */
class Player;
class EnemyMelee;
class Map;
class Light;

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
	int weaponType = 0;
	sf::Texture *playerTexture;
	sf::Texture *enemyMeleeTexture;
	sf::Texture *bulletTexture;
	sf::Texture *laserBeamTexture;
	sf::Texture *pelletTexture;
	sf::Texture *pistolTexture;
	sf::Texture *laserRifleTexture;
	sf::Texture *shotgunTexture;
	sf::Texture *cursorTexture;
	sf::Sprite *cursorSprite;
	sf::View *playerView;
	sf::View *backgroundView;
	sf::View fixed;
	sf::Vector2i mouse;
	std::vector<ProjectileSprite> projectiles;
	std::vector<ProjectileSprite> enemyProjectiles;
	std::vector<EnemyMelee> enemies;
	std::vector<Weapon> weapons;
	/* Temporary time fix, check this from main.cpp */
	static const sf::Time TimePerFrameTmp;

	MapGenerator *mapGenerator;
	Map *map;
	Light *light;
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
	void shoot();
	void spawnEnemies(int amount);
	void checkProjectileCollisions();
	void checkEnemyProjectileCollisions();
	int checkEnemyCollisions(int x, int y, int damage);
	int checkPlayerCollisions(int x, int y, int damage);
	int checkProximity(sf::Vector2f enemy);
	void loadCharacterTextures();
	void loadCursorTexture();
	void loadProjectileTextures();
	void loadWeaponTextures();
	void initializeView();
	void initializeLighting();
	void initializeWeapons();
	void updateLighting();
	void updateEnemies();
	void updateWeapons();
	void updateProjectiles();
	void drawCursor();
	void drawPlayer();
	void drawEnemies();
	void drawWeapons();
	void drawProjectiles();
	sf::Vector2f randomSpawn();
};
extern float frameClock;
extern float lastClockTmp;
extern sf::RenderWindow *app;
extern Game *game;
extern sf::Clock timer;

#endif
