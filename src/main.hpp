/*
 * Dark Domains Of Space
 * 2015 � Project Team (see: LICENSE)
 */

#ifndef MAIN
#define MAIN

#include "gamestate.hpp"

/* Forward declarations due to cyclic dependencies */
class Event;
class Audio;
class Map;
class MapGenerator;
class Light;
class LightState;
class ProjectileSprite;
class Weapon;
class HealthBar;
class ShieldBar;
class Random;
class Pathfinding;
class Player;
class Enemy;
class Valuable;
class Blood;


class Game : public GameState
{
public:
	float tempClock = 0.0;
	bool isPlaying = false;
	long currentClock = 0;
	bool focused;
	bool pathfindAlustus = false;
	int weaponType = 0;
	int ammo;
	int score = 0;
	float zoomLevel;
	float playerPositionX, playerPositionY;
	float weaponHUDX, weaponHUDY;
	float healthbarPositionX, healthbarPositionY;
	float shieldbarPositionX, shieldbarPositionY;
	float healthTextPositionX, healthTextPositionY;
	float scoreTextPositionX, scoreTextPositionY;
	float ammoHUDX, ammoHUDY;
	sf::Texture *playerTexture;
	sf::Texture *enemyMeleeTexture;
	sf::Texture *bulletTexture;
	sf::Texture *laserBeamTexture;
	sf::Texture *pelletTexture;
	sf::Texture *heavyBulletTexture;
	sf::Texture *plasmaBallTexture;
	sf::Texture *beamTexture;
	sf::Texture *weaponTexture;
	sf::Texture *cursorTexture;
	sf::Texture *healthTexture;
	sf::Texture *shieldTexture;
	sf::Texture *valuableTexture;
	sf::Texture *blood8x8Texture;
	sf::Texture *blood16x16Texture;
	sf::Sprite *cursorSprite;
	sf::View *playerView;
	sf::View *backgroundView;
	sf::View fixed;
	sf::Vector2i mouse;
	sf::Font font;
	sf::Text healthText;
	sf::Text scoreText;
	sf::Text currentGun;
	sf::Text currentAmmo;
	std::vector<ProjectileSprite> projectiles;
	std::vector<ProjectileSprite> enemyProjectiles;
	std::vector<Enemy*> enemies;
	std::vector<Weapon*> weapons;
	std::vector<Weapon*> mapWeapons;
	std::vector<Valuable*> mapValuables;
	std::vector<Blood*> mapBlood;

	Random *random;
	MapGenerator *mapGenerator;
	Map *map;
	Audio *audio;
	Light *light;
	LightState *lightState;
	Player *player;
	HealthBar *healthbar;
	ShieldBar *shieldbar;
	StateManager *stateManager;
	Pathfinding *pathfinding;

	Game(StateManager *stateManager);
	~Game();
	void draw();
	void update();
	void handleInput();
	void playFootstepSound();
	void loadTextures();
	void initializeView();
	void initializeLighting();
	void initializeWeapons();
	void initializeHUD();
	void spawnEnemies(int amount);
	void spawnWeapons(int amount);
	void spawnValuables(int totalValue);
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
	void checkEnemiesInAoE(int x, int y, int aoe, int damage);
	void drawEnemies();
	void drawProjectiles();
	void drawPlayer();
	void drawWeapon();
	void drawMapWeapons();
	void drawMapValuables();
	void drawHealthbar();
	void drawShieldBar();
	void drawHUDText();
	void drawCurrentGun();
	void drawCurrentAmmo();
	void drawGore();
	void drawCursor();
	void shoot();
	void dropBlood(sf::Vector2f coords);
	void dropWeapon();
	void pickWeapon();
	void pickValuables();
	void HUDManager();
	int checkProximity(sf::Vector2f enemy);
	int getDistance(int _x0, int _y0, int _x1, int _y1);
	sf::Vector2f randomSpawn();
	sf::Vector2f playerSpawn();
	void clearVectors();
	void createNewStage();
	void checkPlayerDeath();
	std::vector<sf::Vector2f> getPath(sf::Vector2f enemyCoords);
	void endGame();
	void returnToMainMenu();
};

#endif
