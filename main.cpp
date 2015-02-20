/*
 * Project Name
 * 2015 © Project Team (see: LICENSE)
 */

#include <SFML/Graphics.hpp>
#include "fog.hpp"
#include "map.hpp"
#include "object.hpp"
#include "main.hpp"
#include <stdio.h>

/* TODO: Parempi paikka tälle? */
GameState state;

float frameClock;
sf::RenderWindow *app;
Game *game;
sf::Sprite mPlayer;

Game::Game()
{
	clock = new sf::Clock;
	app = new sf::RenderWindow(sf::VideoMode(800, 608, 32),
	                           "SFML Lighting demo by Achpile",
	                           sf::Style::Close);
	running = true;
	lastClock = clock->getElapsedTime().asMilliseconds();
	object = new Object;
	map = new Map;
	app->setFramerateLimit(60);
	state.brush.type = stStatic;
	state.brush.color = sf::Color::Red;
	state.brush.intensity = LIGHT_MAX_LIGHTLEVEL;
	state.brush.sourceTime = 2.0f;
	state.ambientColor = sf::Color::White;
	state.ambientIntensity = 5;
}

Game::~Game()
{
	delete map;
	delete clock;
	delete object;
	delete app;
}

void Game::update()
{
	currentClock = clock->getElapsedTime().asMilliseconds();
	frameClock = (currentClock - lastClock) / 1000.0;
	lastClock = currentClock;

	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	const sf::Time TimePerFrame = sf::seconds(1.f / 60.f);

	state.ambientIntensity = 0;
	state.ambientColor.r = 0;
	state.ambientColor.g = 0;
	state.ambientColor.b = 0;
	state.brush.intensity = 100;
	state.brush.color.r = 150;
	state.brush.color.g = 150;
	state.brush.color.b = 150;
	state.brush.sourceTime = 2.0f;

	/* Options are: stStatic, stPulsing, stFading */
	state.brush.type = stStatic;

	while (running) {
		/* FIXME TILE_SIZE */
		float positionx = mPlayer.getPosition().x;
		printf("\n%f", positionx);
		float positiony = mPlayer.getPosition().y;
		printf("%f\n", positiony);
		state.brush.position = sf::Vector2i((int)positionx/4,
		                                    (int)positiony/4);

		state.tmpSource = StaticLightSource(state.brush.position,
		                                    state.brush.color,
		                                    state.brush.intensity);
		map->ambientColor = state.ambientColor;
		map->ambientIntensity = state.ambientIntensity;

		processEvents();

		sf::Time elapsedTime = clock->restart();
		timeSinceLastUpdate += elapsedTime;
		while (timeSinceLastUpdate > TimePerFrame) {
			timeSinceLastUpdate -= TimePerFrame;
			object->update(TimePerFrame);
		}

		app->clear();
		render();
		app->display();
	}
}

void Game::render()
{
	map->update(&state.tmpSource);
	object->render();
}

void Game::processEvents()
{
	sf::Event event;
	while (app->pollEvent(event)) {
		processEvent(event);
		object->processEvent(event);
	}
	if (!app->isOpen()) {
		running = false;
	}
}

void Game::processEvent(sf::Event event)
{
	switch(event.type) {
	case sf::Event::MouseButtonReleased:
		if (event.mouseButton.button == sf::Mouse::Left) addSource();
		if (event.mouseButton.button == sf::Mouse::Right) map->clear();
		break;
	case sf::Event::Closed:
		stop();
		break;
	}
}

void Game::addSource()
{
	if (! sf::IntRect(0, 0, 800, 608).contains(sf::Mouse::getPosition(*app))) {
		 return;
	}
	switch (state.brush.type) {
	case stStatic:
		map->sources.push_back((StaticLightSource *)
		                       (new StaticLightSource
		                       (state.brush.position,
		                        state.brush.color,
		                        state.brush.intensity)));
		break;
	case stFading:
		map->sources.push_back((StaticLightSource *)
		                       (new FadingLightSource
                                       (state.brush.position,
		                        state.brush.color,
		                        state.brush.intensity,
		                        state.brush.sourceTime)));
		break;
	case stPulsing:
		map->sources.push_back((StaticLightSource *)
		                       (new PulsingLightSource
		                       (state.brush.position,
		                        state.brush.color,
		                        state.brush.intensity,
		                        state.brush.sourceTime)));
		break;
	}
}

int main()
{
	game = new Game;
	srand(time(NULL));
	while(game->running) {
		game->update();
	}
	delete game;
	return EXIT_SUCCESS;
}
