/*
 * Dark Domains Of Space
 * 2015 © Project Team (see: LICENSE)
 */

#include <SFML/Graphics.hpp>
#include <cmath>
#include "map.hpp"
#include "main.hpp"
#include "light.hpp"

Light::Light()
{
	int lightCount = MAP_SIZE_X * MAP_SIZE_Y;
	for (int i = 0; i < LIGHT_MAX_LIGHTLEVEL; i++) {
		lightTiles[i] = new MapTile*[lightCount];
	}
}

Light::~Light()
{
	for (int i = 0; i < LIGHT_MAX_LIGHTLEVEL; delete lightTiles[i++]);
	deleteList(sources);
}

void Light::initialize()
{
	updateList(sources);
	resetLight();
}

void Light::update(StaticLightSource *tmpSource)
{
	checkSources(tmpSource);
	buildLight();
	renderLight();
}

void Light::resetLight()
{
	sf::Vector2i from(0, 0);
	sf::Vector2i to(MAP_SIZE_X, MAP_SIZE_Y);
	sf::Color color = applyIntensity(ambientColor, ambientIntensity);
	for (int i = 0; i < LIGHT_MAX_LIGHTLEVEL; lightCounts[i++] = 0);
	for (int i = from.x; i < to.x; i++) {
		for (int j = from.y; j < to.y; j++) {
			if (game->map->tiles[i][j].type == mtAir) {
				game->map->tiles[i][j].intensity = ambientIntensity;
				game->map->tiles[i][j].light = color;
			} else {
				game->map->tiles[i][j].intensity = 0;
				game->map->tiles[i][j].light = sf::Color::Black;
			}
		}
	}
}

void Light::checkSources(StaticLightSource *tmpSource)
{
	for (unsigned int i = 0; i < sources.size(); i++) {
		addIntensity(sources[i]->position, sources[i]->getIntensity(), sources[i]->color);
	}
	if (sf::IntRect(0, 0, MAP_SIZE_X, MAP_SIZE_Y).contains(tmpSource->position)) {
		addIntensity(tmpSource->position, tmpSource->intensity, tmpSource->color);
	}
}

void Light::buildLight()
{
	sf::Vector2i from(0, 0);
	sf::Vector2i to(MAP_SIZE_X, MAP_SIZE_Y);
	for (int i = from.x; i < to.x; i++) {
		for (int j = from.y; j < to.y; j++) {
			initIntensity(&game->map->tiles[i][j]);
		}
	}
	for (int i = LIGHT_MAX_LIGHTLEVEL - 1; i >= 0; i--) {
		for (int j = 0; j < lightCounts[i]; j++) {
			if (lightTiles[i][j]->intensity != i + 1) {
				continue;
			}
			checkNeighbours(lightTiles[i][j]);
		}
	}
}

void Light::renderLight()
{
	sf::Vector2i from(0, 0);
	sf::Vector2i to(MAP_SIZE_X, MAP_SIZE_Y);
	for (int i = from.x - 1; i < to.x; i++) {
		for (int j = from.y - 1; j < to.y; j++) {
			lightMask[0].position = getTilePos(i, j);
			lightMask[1].position = getTilePos(i + 1, j);
			lightMask[2].position = getTilePos(i + 1, j + 1);
			lightMask[3].position = getTilePos(i, j + 1);
			lightMask[0].color = getTileLight(i, j);
			lightMask[1].color = getTileLight(i + 1, j);
			lightMask[2].color = getTileLight(i + 1, j + 1);
			lightMask[3].color = getTileLight(i, j + 1);
			app->draw(lightMask, 4, sf::Quads, sf::BlendMultiply);
		}
	}
}

void Light::initIntensity(MapTile *tile)
{
	int index = tile->intensity - 1;
	if (index < 0 || index >= LIGHT_MAX_LIGHTLEVEL) {
		return;
	}
	lightTiles[index][lightCounts[index]++] = tile;
}

void Light::setIntensity(MapTile *tile, char intensity, sf::Color color)
{
	if (intensity > tile->intensity || canMixColors(tile->light, color)) {
		tile->light = mixColors(tile->light, color);
		if (intensity != tile->intensity) {
			tile->intensity = intensity;
			int index = tile->intensity - 1;
			if (index < 0) {
				return;
			}
			if (index >= LIGHT_MAX_LIGHTLEVEL) {
				return;
			}
			lightTiles[index][lightCounts[index]] = tile;
			lightCounts[index]++;
		}
	}
}

void Light::addIntensity(sf::Vector2i index, char intensity, sf::Color color)
{
	if (index.x < 0 || index.x >= MAP_SIZE_X || index.y < 0 || index.y >= MAP_SIZE_X) {
		return;
	}
	color = applyIntensity(color, intensity);
	game->map->tiles[index.x][index.y].light = mixColors(game->map->tiles[index.x][index.y].light, color);
	if (game->map->tiles[index.x][index.y].intensity < intensity) {
		game->map->tiles[index.x][index.y].intensity = intensity;
	}
}

sf::Color Light::applyIntensity(sf::Color c, char intensity)
{
	float k;
	if (intensity >= LIGHT_ABSOLUTE) {
		k = 1.0f;
	} else {
		k = (float)intensity / (float)LIGHT_ABSOLUTE;
	}
	sf::Color result;
	result.r = (char)(c.r * k);
	result.g = (char)(c.g * k);
	result.b = (char)(c.b * k);
	result.a = 255;
	return result;
}

sf::Color Light::reapplyIntensity(sf::Color c, char intensityOld, char intensityNew)
{
	float k1, k2;
	if (intensityNew >= LIGHT_ABSOLUTE) {
		k1 = 1.0f;
	} else {
		k1 = (float)intensityNew / (float)LIGHT_ABSOLUTE;
	}

	if (intensityOld >= LIGHT_ABSOLUTE) {
		k2 = 1.0f;
	} else {
		k2 = (float)intensityOld / (float)LIGHT_ABSOLUTE;
	}
	sf::Color result;
	result.r = (char)(c.r * k1 / k2);
	result.g = (char)(c.g * k1 / k2);
	result.b = (char)(c.b * k1 / k2);
	result.a = 255;
	return result;
}

void Light::checkNeighbours(MapTile *tile)
{
	int x = tile->index.x;
	int y = tile->index.y;
	char intensity = tile->intensity - tile->absorb;
	if (intensity < 0) {
		return;
	}
	sf::Color color = reapplyIntensity(tile->light, tile->intensity, intensity);
	if (x > 0) {
		setIntensity(&game->map->tiles[x - 1][y], intensity, color);
	}
	if (x < MAP_SIZE_X - 1) {
		setIntensity(&game->map->tiles[x + 1][y], intensity, color);
	}
	if (y > 0) {
		setIntensity(&game->map->tiles[x][y - 1], intensity, color);
	}
	if (y < MAP_SIZE_Y - 1) {
		setIntensity(&game->map->tiles[x][y + 1], intensity, color);
	}
	color.r *= 0.9f;
	color.g *= 0.9f;
	color.b *= 0.9f;
	if (x > 0 && y < MAP_SIZE_Y - 1) {
		setIntensity(&game->map->tiles[x - 1][y + 1], intensity, color);
	}
	if (x < MAP_SIZE_X - 1 && y > 0) {
		setIntensity(&game->map->tiles[x + 1][y - 1], intensity, color);
	}
	if (y > 0 && x > 0) {
		setIntensity(&game->map->tiles[x - 1][y - 1], intensity, color);
	}
	if (y < MAP_SIZE_Y - 1 && x < MAP_SIZE_X - 1) {
		setIntensity(&game->map->tiles[x + 1][y + 1], intensity, color);
	}
}

bool Light::canMixColors(sf::Color base, sf::Color light)
{
	if (light.r > base.r) return true;
	if (light.g > base.g) return true;
	if (light.b > base.b) return true;
	return false;
}

sf::Color Light::mixColors(sf::Color c1, sf::Color c2)
{
	sf::Color result;
	result.a = 255;
	result.r = (c1.r > c2.r) ? c1.r : c2.r;
	result.g = (c1.g > c2.g) ? c1.g : c2.g;
	result.b = (c1.b > c2.b) ? c1.b : c2.b;
	return result;
}

sf::Vector2f Light::getTilePos(int x, int y)
{
	return sf::Vector2f(TILE_SIZE / 2.0f + TILE_SIZE * x, TILE_SIZE / 2.0f + TILE_SIZE * y);
}

sf::Color Light::getTileLight(int x, int y)
{
	if (x < 0) {
		x = 0;
	}
	if (y < 0) {
		y = 0;
	}
	if (x >= MAP_SIZE_X) {
		x = MAP_SIZE_X - 1;
	}
	if (y >= MAP_SIZE_Y) {
		y = MAP_SIZE_Y - 1;
	}
	return game->map->tiles[x][y].light;
}

void Light::addSource(SourceType type)
{
	switch (type) {
	case stStatic:
		sources.push_back((StaticLightSource *)
		                  (new StaticLightSource
		                  (game->lightState->brush.position,
		                   game->lightState->brush.color,
		                   game->lightState->brush.intensity)));
		break;
	case stFading:
		sources.push_back((StaticLightSource *)
		                  (new FadingLightSource
		                  (game->lightState->brush.position,
		                   game->lightState->brush.color,
		                   game->lightState->brush.intensity,
		                   game->lightState->brush.sourceTime)));
		break;
	case stPulsing:
		sources.push_back((StaticLightSource *)
		                  (new PulsingLightSource
		                  (game->lightState->brush.position,
		                   game->lightState->brush.color,
		                   game->lightState->brush.intensity,
		                   game->lightState->brush.sourceTime)));
		break;
	case stTest:
		sources.push_back((StaticLightSource *)
		                  (new TestLightSource
		                  (game->lightState->brush.position,
		                   game->lightState->brush.color,
		                   game->lightState->brush.intensity,
		                   game->lightState->brush.sourceTime)));
		break;
	}
}

float Light::sqr(float x)
{
	return x * x;
}

StaticLightSource::StaticLightSource(sf::Vector2i _position, sf::Color _color, char _intensity)
{
	position = _position;
	color = _color;
	intensity = _intensity;
	actualIntensity = _intensity;
}

FadingLightSource::FadingLightSource(sf::Vector2i _position, sf::Color _color,
                                     char _intensity, float _lifetime)
                                     : StaticLightSource(_position, _color, _intensity)
{
	lifetime = _lifetime;
}

PulsingLightSource::PulsingLightSource(sf::Vector2i _position, sf::Color _color,
                                       char _intensity, float _period)
                                       : StaticLightSource(_position, _color, _intensity)
{
	period = _period;
}

TestLightSource::TestLightSource(sf::Vector2i _position, sf::Color _color,
                                 char _intensity, float _period)
                                 : StaticLightSource(_position, _color, _intensity)
{
	period = _period;
}

bool FadingLightSource::update()
{
	life += frameClock;
	actualIntensity = (char)(intensity * (lifetime - life) / lifetime);
	return !over();
}

bool PulsingLightSource::update()
{
	life += frameClock;
	actualIntensity = (char)abs(intensity * (cos(3.1415926 * (life / period))));
	return !over();
}

bool TestLightSource::update()
{
	life += frameClock;
	actualIntensity = (char)abs(intensity * (sin(life)*0.5 + sin(3 * life)*0.2 + sin(0.3*life)*0.5));
	return !over();
}
