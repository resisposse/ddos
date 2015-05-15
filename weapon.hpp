/*
* Dark Domains Of Space
* 2015 © Project Team (see: LICENSE)
*/

#ifndef WEAPON
#define WEAPON

class Weapon
{
public:
	std::string name;
	int ammoType;
	int bullets;
	int spreadAngle;
	float attackSpeed;
	sf::Sprite sprite;
	/* Must be the position where the weapons is in the weapons vector
	 * in initializeWeapons function in main! */
	int weaponPosition;

	Weapon(sf::Texture& weaponTexture);
	void update(float playerPositionX, float playerPositionY, float mouseX, float mouseY);
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
