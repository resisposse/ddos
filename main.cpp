/*
 * Project Name
 * 2015 ? Project Team (see: LICENSE)
 */

#include <SFML/Graphics.hpp>
#include "fog.hpp"
#include "map.hpp"
#include "object.hpp"
#include "main.hpp"

float frameClock = 0;
GameState state;
sf::RenderWindow *app;
sf::View *playerView;
Game *game;
sf::Clock timer;

Game::Game()
{
	app = new sf::RenderWindow(sf::VideoMode(800, 608, 32),
	                           "Dark Domains Of Space",
	                           sf::Style::Resize | sf::Style::Close);
	running = true;
	lastClock = timer.getElapsedTime().asMilliseconds();
	object = new Object;
	map = new Map;
	app->setFramerateLimit(60);
	state.brush.type = stStatic;
	state.brush.color = sf::Color::Red;
	state.brush.intensity = LIGHT_MAX_LIGHTLEVEL;
	state.brush.sourceTime = 2.0f;
	state.ambientColor = sf::Color::White;
	state.ambientIntensity = 5;
	
	


	this->zoomLevel = 1.0f;
	playerView = new sf::View;
	
	float positionX = mPlayerSpr->getPosition().x;
	float positionY = mPlayerSpr->getPosition().y;
	
	playerView->setCenter(positionX, positionY);
	playerView->setSize(sf::Vector2f(600, 600));
	app->setView(*playerView);

	
	
}

Game::~Game()
{
	delete map;
	delete object;
	delete app;
	delete playerView;
	
}

void Game::update()
{
	state.ambientIntensity = 0;
	state.ambientColor.r = 0;
	state.ambientColor.g = 0;
	state.ambientColor.b = 0;
	state.brush.intensity = 100;
	state.brush.color.r = 150;
	state.brush.color.g = 150;
	state.brush.color.b = 150;
	state.brush.sourceTime = 2.0f;

	/* Options are: stStatic, stPulsing, stFading, stTest */
	state.brush.type = stPulsing;

	while (running) {
		currentClock += timer.getElapsedTime().asMilliseconds();
		frameClock = (currentClock - lastClock) / 1000.f;
		lastClock = currentClock;

		float positionX = mPlayerSpr->getPosition().x;
		float positionY = mPlayerSpr->getPosition().y;
		state.brush.position = sf::Vector2i((int)positionX / TILE_SIZE,
		                                    (int)positionY / TILE_SIZE);
		state.tmpSource = StaticLightSource(state.brush.position,
		                                    state.brush.color,
		                                    state.brush.intensity);
		map->ambientColor = state.ambientColor;
		map->ambientIntensity = state.ambientIntensity;

		processEvents();
		object->run();
		app->clear();
		render();
		playerView->setCenter(positionX, positionY);
		map->bgSpr->setOrigin(400, 300);
		map->bgSpr->setPosition(positionX, positionY);
		app->setView(*playerView);
		
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

		if (event.type == sf::Event::Resized)
		{
			//sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
			//app->setView(sf::View(visibleArea));

			playerView->setSize(event.size.width, event.size.height);
			map->bgSpr->setPosition(app->mapPixelToCoords(sf::Vector2i(0, 0), *playerView));
			sf::Vector2f pos = sf::Vector2f(event.size.width, event.size.height);
			pos *= 0.5f;
			pos = app->mapPixelToCoords(sf::Vector2i(pos), *playerView);

			map->bgSpr->setScale(
				float(event.size.width) / float(map->bgSpr->getTexture()->getSize().x),
				float(event.size.height) / float(map->bgSpr->getTexture()->getSize().y));
			break;
		}
	}
	if (!app->isOpen()) {
		running = false;
	}
}

void Game::processEvent(sf::Event event)
{
	switch(event.type) {

	case sf::Event::MouseWheelMoved:
	{
		if (event.mouseWheel.delta < 0)
		{
			if (zoomLevel < 2.0f) {
				playerView->zoom(2.0f);
				zoomLevel *= 2.0f;
			}
		}
		else
		{
			if (zoomLevel > 0.25f) {
				playerView->zoom(0.5f);
				zoomLevel *= 0.5f;
			}
		}
	}


	case sf::Event::KeyPressed:
	{
		if (event.key.code == sf::Keyboard::Escape) app->close();
		break;
	}


	case sf::Event::MouseButtonReleased:
		if (event.mouseButton.button == sf::Mouse::Left) addSource();
		if (event.mouseButton.button == sf::Mouse::Right) map->clear();
		break;
	case sf::Event::Closed:
		running = false;
		break;
	}
}

void Game::addSource()
{
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
	case stTest:
		map->sources.push_back((StaticLightSource *)
		                       (new TestLightSource
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
	while (game->running) {
		game->update();
	}
	delete game;
	return EXIT_SUCCESS;
}
