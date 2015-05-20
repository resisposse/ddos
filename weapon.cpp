/*
* Dark Domains Of Space
* 2015 © Project Team (see: LICENSE)
*/

#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include "object.hpp"
#include "main.hpp"
#include "map.hpp"
#include "weapon.hpp"

/*
 * Ammo types for weapons
 * 0 = bullet
 * 1 = laser
 * 2 = pellet
 */
Weapon::Weapon(sf::Texture& weaponTexture)
{
	sprite.setTexture(weaponTexture);
}

void Weapon::update(float playerPositionX, float playerPositionY, float mouseX, float mouseY)
{
	float angle;
	double a, b;

	a = mouseX - (playerPositionX);
	b = mouseY - (playerPositionY);
	angle = -atan2(a, b) * 180 / 3.141593;
	sprite.setRotation(angle);
	sprite.setPosition(playerPositionX, playerPositionY);
}

void Weapon::setAmmo(int _ammo)
{
	int currentAmmo = getAmmo();
	int newAmmo = currentAmmo - _ammo;
	if (newAmmo < 0) newAmmo = 0;
	ammo = newAmmo;
}

int Weapon::getAmmo() const
{
	return ammo;
}

Pistol::Pistol(sf::Texture& weaponTexture) : Weapon(weaponTexture)
{
	name = "Pistol";
	attackSpeed = 0.2;
	spreadAngle = 15;
	ammoType = 0;
	ammo = 30;
	bullets = 1;
	weaponPosition = 0;

	sf::IntRect mPistol(32 * 2, 32 * 0, 32, 32);
	sprite.setTextureRect(mPistol);
	sprite.setOrigin(16, 0);
}

LaserRifle::LaserRifle(sf::Texture& weaponTexture) : Weapon(weaponTexture)
{
	name = "Lasergun";
	attackSpeed = 0.3;
	spreadAngle = 5;
	ammoType = 1;
	ammo = 30;
	bullets = 1;
	weaponPosition = 1;

	sf::IntRect mLaserRifle(32 * 3, 32 * 0, 32, 32);
	sprite.setTextureRect(mLaserRifle);
	sprite.setOrigin(16, 0);
}

Shotgun::Shotgun(sf::Texture& weaponTexture) : Weapon(weaponTexture)
{
	name = "Shotgun";
	attackSpeed = 1;
	spreadAngle = 20;
	ammoType = 2;
	ammo = 30;
	bullets = 7;
	weaponPosition = 2;

	sf::IntRect mShotgun(32 * 1, 32 * 0, 32, 32);
	sprite.setTextureRect(mShotgun);
	sprite.setOrigin(16, 0);
}
