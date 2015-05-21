/*
 * Dark Domains Of Space
 * 2015 © Project Team (see: LICENSE)
 */

#ifndef LIGHT
#define LIGHT

#include "globals.hpp"

/* Forward declaration due to a cyclic dependency */
class MapTile;

enum SourceType
{
	stStatic,
	stFading,
	stPulsing,
	stTest
};

class StaticLightSource
{
public:
	char intensity;
	char actualIntensity;
	sf::Vector2i position;
	sf::Color color;

	StaticLightSource() {};
	StaticLightSource(sf::Vector2i _position, sf::Color _color, char _intensity);
	virtual ~StaticLightSource() {};
	virtual bool update() { return !over(); }
	virtual char getIntensity() { return actualIntensity; }
	virtual bool over() { return false; }
};

class Light
{
public:
	int lightCounts[LIGHT_MAX_LIGHTLEVEL];
	char ambientIntensity;
	sf::Color ambientColor;
	sf::Vertex lightMask[4];
	std::vector<StaticLightSource *> sources;
	MapTile **lightTiles[LIGHT_MAX_LIGHTLEVEL];

	Light();
	~Light();
	void initialize();
	void update(StaticLightSource *tmpSource);
	void resetLight();
	void checkSources(StaticLightSource *tmpSource);
	void buildLight();
	void renderLight();
	void initIntensity(MapTile *tile);
	void setIntensity(MapTile *tile, char intensity, sf::Color color);
	void addIntensity(sf::Vector2i index, char intensity, sf::Color color);
	sf::Color applyIntensity(sf::Color c, char intensity);
	sf::Color reapplyIntensity(sf::Color c, char intensity1, char intensity2);
	void checkNeighbours(MapTile *tile);
	bool canMixColors(sf::Color base, sf::Color light);
	sf::Color mixColors(sf::Color c1, sf::Color c2);
	sf::Vector2f getTilePos(int x, int y);
	sf::Color getTileLight(int x, int y);
	void addSource(SourceType type);
	float sqr(float x);
	void clear() { deleteList(sources); };
};

class FadingLightSource : StaticLightSource
{
public:
	float lifetime;
	float life = 0;

	FadingLightSource(sf::Vector2i _position, sf::Color _color,
	                  char _intensity, float _lifetime);
	bool update();
	bool over() { return (life > lifetime); }
};

class PulsingLightSource : StaticLightSource
{
public:
	float period;
	float life = 0;

	PulsingLightSource(sf::Vector2i _position, sf::Color _color,
	                   char _intensity, float _period);
	bool update();
	bool over() { return false; }
};

class TestLightSource : StaticLightSource
{
public:
	float period;
	float life = 0;

	TestLightSource(sf::Vector2i _position, sf::Color _color,
	                char _intensity, float _period);
	bool update();
	bool over() { return false; }
};

class Brush
{
public:
	SourceType type;
	sf::Color color;
	sf::Vector2i position;
	char intensity;
	float sourceTime;
};

class LightState
{
public:
	Brush brush;
	char ambientIntensity;
	sf::Color ambientColor;
	StaticLightSource tmpSource;
};

#endif
