/*
 * Dark Domains Of Space
 * 2015 © Project Team (see: LICENSE)
 */

#include <SFML/Graphics.hpp>
#include "main.hpp"
#include "map.hpp"
#include "light.hpp"
#include "weapon.hpp"
#include "object.hpp"
#include "event.hpp"

/*
 * We have designed our way into this in a way where you have to call everything
 * through our global Game object. We are then able to use every object created
 * in the Game constructor and their respective functions.
 */
void Event::processEvent()
{
	sf::Event event;
	while (app->pollEvent(event)) {
		switch (event.type) {
		case sf::Event::KeyPressed:
			switch (event.key.code) {
			case sf::Keyboard::W:
				game->player->mIsMovingUp = true;
				break;
			case sf::Keyboard::A:
				game->player->mIsMovingLeft = true;
				break;
			case sf::Keyboard::S:
				game->player->mIsMovingDown = true;
				break;
			case sf::Keyboard::D:
				game->player->mIsMovingRight = true;
				break;
			case sf::Keyboard::E:
				game->heldWeapon = (game->heldWeapon + 1) % game->playerWeapons.size();
				break;
			case sf::Keyboard::Q:
				if (game->heldWeapon == 0) {
					game->heldWeapon = game->playerWeapons.size();
				}
				game->heldWeapon = (game->heldWeapon - 1);
				break;
			case sf::Keyboard::G:
				game->dropWeapon();
				game->pickWeapon();
				break;
			case sf::Keyboard::Escape:
				app->close();
				break;
			default:
				break;
			}
			break;
		case sf::Event::KeyReleased:
			switch (event.key.code) {
			case sf::Keyboard::W:
				game->player->mIsMovingUp = false;
				break;
			case sf::Keyboard::A:
				game->player->mIsMovingLeft = false;
				break;
			case sf::Keyboard::S:
				game->player->mIsMovingDown = false;
				break;
			case sf::Keyboard::D:
				game->player->mIsMovingRight = false;
				break;
			default:
				break;
			}
			break;
		case sf::Event::MouseButtonPressed:
			switch (event.mouseButton.button) {
			case sf::Mouse::Left:
				game->shooting = true;
				break;
			case sf::Mouse::Right:
				game->spawnEnemies(1);
				break;
			default:
				break;
			}
			break;
		case sf::Event::MouseButtonReleased:
			switch (event.mouseButton.button) {
			case sf::Mouse::Left:
				game->shooting = false;
				break;
			case sf::Mouse::Right:
				game->light->clear();
				break;
			default:
				break;
			}
			break;
		case sf::Event::MouseWheelMoved:
			if (event.mouseWheel.delta < 0) {
				if (game->zoomLevel < 2.0f) {
					game->playerView->zoom(2.0f);
					game->zoomLevel *= 2.0f;
				}
			} else {
				if (game->zoomLevel > 0.25f) {
					game->playerView->zoom(0.5f);
					game->zoomLevel *= 0.5f;
				}
			}
			break;
		case sf::Event::Closed:
			game->running = false;
			break;
		case sf::Event::Resized:
			game->playerView->setSize(event.size.width, event.size.height);
			game->map->bgSpr->setPosition(app->mapPixelToCoords(sf::Vector2i(0, 0),
			                                                    *game->playerView));
			pos = sf::Vector2f(event.size.width, event.size.height);
			pos *= 0.5f;
			pos = app->mapPixelToCoords(sf::Vector2i(pos), *game->playerView);
			game->map->bgSpr->setScale(float(event.size.width) / float(game->map->bgSpr->getTexture()->getSize().x),
			                           float(event.size.height) / float(game->map->bgSpr->getTexture()->getSize().y));
			break;
		default:
			break;
		}
	}
	if (!app->isOpen()) {
		game->running = false;
	}
}
