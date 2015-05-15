/*
 * Dark Domains Of Space
 * 2015 © Project Team (see: LICENSE)
 */

#ifndef LIGHT
#define LIGHT

#define LIGHT_MAX_LIGHTLEVEL 100
#define LIGHT_ABSOLUTE 80

#define deleteList(list) \
for (; !(list).empty(); delete (list).back(), (list).pop_back())

#define updateList(list) \
for (unsigned int i = 0; i < (list).size();) { \
	if (!(list)[i]->update()) { \
		delete (list)[i]; \
		(list).erase((list).begin() + i); \
	} else { \
		i++; \
	} \
}

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
	void resetLight();
	void buildLight();
	void renderLight();
	void initIntensity(MapTile *tile);
	void setIntensity(MapTile *tile, char intensity, sf::Color color);
	void addIntensity(sf::Vector2i index, char intensity, sf::Color color);
	void checkNeighbours(MapTile *tile);
	void initialize();
	void update(StaticLightSource *tmpSource);
	void checkSources(StaticLightSource *tmpSource);
	void clear() { deleteList(sources); };
	bool canMixColors(sf::Color base, sf::Color light);
	float sqr(float x);
	sf::Vector2f getTilePos(int x, int y);
	sf::Color getTileLight(int x, int y);
	sf::Color applyIntensity(sf::Color c, char intensity);
	sf::Color reapplyIntensity(sf::Color c, char intensity1, char intensity2);
	sf::Color mixColors(sf::Color c1, sf::Color c2);
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

class GameState
{
public:
	Brush brush;
	char ambientIntensity;
	sf::Color ambientColor;
	StaticLightSource tmpSource;
};

#endif
