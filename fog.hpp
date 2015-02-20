/*
* Project Name
* 2015 © Project Team (see: LICENSE)
*/

#ifndef FOG
#define FOG

#define LIGHT_MAX_LIGHTLEVEL 100
#define LIGHT_ABSOLUTE 80

sf::Color applyIntensity(sf::Color c, char intensity);
sf::Color reapplyIntensity(sf::Color c, char intensity1, char intensity2);
sf::Color mixColors(sf::Color c1, sf::Color c2);
bool canMixColors(sf::Color base, sf::Color light);
float sqr(float x);
void clearSources();

enum SourceType
{
	stStatic,
	stFading,
	stPulsing,
	stTest
};

struct StaticLightSource
{
	sf::Vector2i position;
	sf::Color color;
	char intensity;
	char actualIntensity;
	StaticLightSource() {};
	StaticLightSource(sf::Vector2i _position, sf::Color _color, char _intensity);
	virtual bool update() {
		return !over();
	};
	virtual char getIntensity() {
		return actualIntensity;
	};
	virtual bool over() {
		return false;
	};
};

struct FadingLightSource : StaticLightSource
{
	float lifetime;
	float life = 0;
	FadingLightSource(sf::Vector2i _position, sf::Color _color,
	                  char _intensity, float _lifetime);
	bool update();
	bool over() {
		return (life > lifetime);
	};
};

struct PulsingLightSource : StaticLightSource
{
	float period;
	float life = 0;
	PulsingLightSource(sf::Vector2i _position, sf::Color _color,
	                   char _intensity, float _period);
	bool update();
	bool over() {
		return false;
	};
};

struct TestLightSource : StaticLightSource
{
	float period;
	float life = 0;
	TestLightSource(sf::Vector2i _position, sf::Color _color,
	                char _intensity, float _period);
	bool update();
	bool over() {
		return false;
	};
};

struct GameState
{
	struct {
		SourceType type;
		sf::Color color;
		sf::Vector2i position;
		char intensity;
		float sourceTime;
	} brush;
	char ambientIntensity;
	sf::Color ambientColor;
	StaticLightSource tmpSource;
};

#endif
