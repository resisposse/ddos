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

	MapGenerator *mapGenerator;
	Map *map;
	Light *light;
	Player *player;
	EnemyMelee *enemy;

	Game();
	~Game();
	int collision(float x, float y, std::string collisionType);
	sf::Vector2f randomSpawn();
	void update();
	void render();
	void parseEvents();
	void processEvent(sf::Event event);
	void addSource();
	void shoot();
	void loadCharacterTextures();
	void loadCursorTexture();
	void loadProjectileTextures();
	void loadWeaponTextures();
	void updateProjectiles();
	void checkProjectileCollisions();
	void checkEnemyProjectileCollisions();
	int checkEnemyCollisions(int x, int y, int damage);
	int checkPlayerCollisions(int x, int y, int damage);
	void drawProjectiles();
	void spawnEnemies(int amount);
	void updateEnemies();
	void updateWeapons();
	int checkProximity(sf::Vector2f enemy);
	void drawEnemies();
	void drawPlayer();
	void drawCursor();
	void drawWeapons();
	void initializeView();
	void initializeLighting();
	void updateLighting();
	void initializeWeapons();

	sf::Texture *playerTexture;
	sf::Texture *enemyMeleeTexture;
	sf::Texture *bulletTexture;
	sf::Texture *laserBeamTexture;
	sf::Texture *pelletTexture;
	sf::Texture *pistolTexture;
	sf::Texture *laserRifleTexture;
	sf::Texture *shotgunTexture;
	sf::Texture *textureCursor;
	sf::Sprite *spriteCursor;
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
};
extern float frameClock;
extern float lastClockTmp;
extern sf::RenderWindow *app;
extern Game *game;
extern sf::Clock timer;

#endif
