/*
 * Dark Domains Of Space
 * 2015 © Project Team (see: LICENSE)
 */

#ifndef MAIN
#define MAIN

/* Forward declarations due to cyclic dependencies */
class Event;
class Map;
class MapGenerator;
class Light;
class LightState;
class Object;
class Player;
class EnemyMelee;
class ProjectileSprite;
class Weapon;
class HealthBar;

class Game
{
public:
	long currentClock = 0;
	bool running;
	bool focused;
	int heldWeapon = 0;
	int weaponType = 0;
	int ammo;
	float zoomLevel;
	float playerPositionX, playerPositionY;
	float weaponHUDX, weaponHUDY;
	float healthbarPositionX, healthbarPositionY;
	float healthTextPositionX, healthTextPositionY;
	float ammoHUDX, ammoHUDY;
	sf::Texture *playerTexture;
	sf::Texture *enemyMeleeTexture;
	sf::Texture *bulletTexture;
	sf::Texture *laserBeamTexture;
	sf::Texture *pelletTexture;
	sf::Texture *pistolTexture;
	sf::Texture *laserRifleTexture;
	sf::Texture *shotgunTexture;
	sf::Texture *cursorTexture;
	sf::Texture *healthTexture;
	sf::Sprite *cursorSprite;
	sf::View *playerView;
	sf::View *backgroundView;
	sf::View fixed;
	sf::Vector2i mouse;
	sf::Font font;
	sf::Text healthText;
	sf::Text currentGun;
	sf::Text currentAmmo;
	std::vector<ProjectileSprite> projectiles;
	std::vector<ProjectileSprite> enemyProjectiles;
	std::vector<Object> enemies;
	std::vector<Weapon> weapons;
	std::vector<Weapon> weaponsOnMap;
	std::vector<Weapon> playerWeapons;

	Event *event;
	MapGenerator *mapGenerator;
	Map *map;
	Light *light;
	LightState *lightState;
	Player *player;
	EnemyMelee *enemy;
	HealthBar *healthbar;

	Game();
	~Game();
	void update();
	void render();
	void loadCursorTexture();
	void loadHealthbarTexture();
	void loadCharacterTextures();
	void loadWeaponTextures();
	void loadProjectileTextures();
	void initializeView();
	void initializeLighting();
	void initializeWeapons();
	void initializeHUD();
	void spawnEnemies(int amount);
	void spawnWeapons(int amount);
	void updateClock();
	void updateView();
	void updateLighting();
	void updatePlayer();
	void updateEnemies();
	void updateWeapons();
	void updateProjectiles();
	void checkProjectileCollisions();
	void checkEnemyProjectileCollisions();
	int checkEnemyCollisions(int x, int y, int damage);
	int checkPlayerCollisions(int x, int y, int damage);
	void drawEnemies();
	void drawProjectiles();
	void drawPlayer();
	void drawWeapon();
	void drawWeaponsOnMap();
	void drawHealthbar();
	void drawHealthText();
	void drawCurrentGun();
	void drawCurrentAmmo();
	void drawCursor();
	void shoot();
	void dropWeapon();
	void pickWeapon();
	void HUDManager();
	int checkProximity(sf::Vector2f enemy);
	sf::Vector2f randomSpawn();
	sf::Vector2f playerSpawn();
};
extern float frameClock;
extern long lastClock;
extern sf::Clock timer;
extern sf::RenderWindow *app;
extern Game *game;

#endif
