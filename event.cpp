/*
 * Dark Domains Of Space
 * 2015 © Project Team (see: LICENSE)
 */

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include "main.hpp"
#include "map.hpp"
#include "light.hpp"
#include "weapon.hpp"
#include "object.hpp"
#include "audio.hpp"
#include "event.hpp"

/*
 * We have designed our way into this in a way where you have to call everything
 * through our global Game object. We are then able to use every object created
 * in the Game constructor and their respective functions.
 */
void Event::processEvent()
{
	sf::Event event;
	clock += frameClock;
	std::cout << clock << "\n";
	playFootstepSound();
	while (app->pollEvent(event)) {
		switch (event.type) {
		case sf::Event::KeyPressed:
			switch (event.key.code) {
			case sf::Keyboard::W:
				game->player->mIsMovingUp = true;
				mIsMovingUpSound = true;
				break;
			case sf::Keyboard::A:
				game->player->mIsMovingLeft = true;
				mIsMovingLeftSound = true;
				break;
			case sf::Keyboard::S:
				game->player->mIsMovingDown = true;
				mIsMovingDownSound = true;
				break;
			case sf::Keyboard::D:
				game->player->mIsMovingRight = true;
				mIsMovingRightSound = true;
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
			case sf::Keyboard::T: {
				int i = (int)game->playerPositionX / 32;
				int j = (int)game->playerPositionY / 32;
				if (game->map->tiles[i][j].type == MapTileType::mtGoal) {
					game->audio->teleportSound->play();
					game->createNewStage();
				}
				break;
			}
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
				mIsMovingUpSound = false;
				break;
			case sf::Keyboard::A:
				game->player->mIsMovingLeft = false;
				mIsMovingLeftSound = false;
				break;
			case sf::Keyboard::S:
				game->player->mIsMovingDown = false;
				mIsMovingDownSound = false;
				break;
			case sf::Keyboard::D:
				game->player->mIsMovingRight = false;
				mIsMovingRightSound = false;
				break;
			default:
				break;
			}
			break;
		case sf::Event::MouseButtonPressed:
			switch (event.mouseButton.button) {
			case sf::Mouse::Left:
				game->player->playerShooting = true;
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
				game->player->playerShooting = false;
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

void Event::playFootstepSound()
{
	if (mIsMovingUpSound == true ||
	    mIsMovingLeftSound == true ||
	    mIsMovingDownSound == true ||
	    mIsMovingRightSound == true) {
		if (isPlaying == false) {
			clock = 0.0;
			random = rand() % 3;
			game->audio->footsteps[random]->play();
			isPlaying = true;
		} else if (isPlaying == true) {
			if (clock > 0.35) {
				isPlaying = false;
			}
		}
	}
}
