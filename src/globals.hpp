/*
 * Dark Domains Of Space
 * 2015 © Project Team (see: LICENSE)
 */

#ifndef GLOBALS
#define GLOBALS

class Game;

/* Deletes all allocated memory from vectors */
#define deleteList(list) \
for (; !(list).empty(); delete (list).back(), (list).pop_back())

/*
 * Used for light sources, where if the light source doesn't go through update()
 * anymore (for example the fading light disappears), the memory is freed.
 */
#define updateList(list) \
for (unsigned int i = 0; i < (list).size();) { \
	if (!(list)[i]->update()) { \
		delete (list)[i]; \
		(list).erase((list).begin() + i); \
	} else { \
		i++; \
	} \
}

#define LIGHT_MAX_LIGHTLEVEL 100
#define LIGHT_ABSOLUTE 80

#define TILE_SIZE 32
#define MAP_SIZE_X 80
#define MAP_SIZE_Y 68

#define WEAPON_VOLUME 3
#define MISC_VOLUME 10
#define MUSIC_VOLUME 10

#define PI 3.1415926535

extern float frameClock;
extern long lastClock;
extern sf::Clock timer;
extern sf::RenderWindow *app;
extern Game *game;

#endif
