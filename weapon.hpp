/*
* Dark Domains Of Space
* 2015 © Project Team (see: LICENSE)
*/

#ifndef WEAPON
#define WEAPON

class Weapon
{
public:
	Weapon(sf::Texture& weaponTexture);
	void update(float timePerFrame, float playerPositionX, float playerPositionY, float mouseX, float mouseY);
	sf::Sprite sprite;
	int ammoType;
	int bullets;
	int spreadAngle;
	float attackSpeed;
};

class Pistol : public Weapon {
public:
	Pistol(sf::Texture& objectTexture);
};

class LaserRifle : public Weapon {
public:
	LaserRifle(sf::Texture& objectTexture);
};

class Shotgun : public Weapon {
public:
	Shotgun(sf::Texture& objectTexture);
};

#endif