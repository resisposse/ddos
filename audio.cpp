/*
 *
 */

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "globals.hpp"
#include "audio.hpp"

Audio::Audio()
{
	bulletBuffer = new sf::SoundBuffer;
	bulletBuffer->loadFromFile("audio/glock.wav");
	bulletSound = new sf::Sound;
	bulletSound->setBuffer(*bulletBuffer);
	bulletSound->setVolume(WEAPON_VOLUME);

	sniperBuffer = new sf::SoundBuffer;
	sniperBuffer->loadFromFile("audio/sniperSound.wav");
	sniperSound = new sf::Sound;
	sniperSound->setBuffer(*sniperBuffer);
	sniperSound->setVolume(WEAPON_VOLUME);

	shotgunBuffer = new sf::SoundBuffer;
	shotgunBuffer->loadFromFile("audio/shotgunCCA3.wav");
	shotgunSound = new sf::Sound;
	shotgunSound->setBuffer(*shotgunBuffer);
	shotgunSound->setVolume(WEAPON_VOLUME);

	laserBuffer = new sf::SoundBuffer;
	laserBuffer->loadFromFile("audio/laser1.ogg");
	laserSound = new sf::Sound;
	laserSound->setBuffer(*laserBuffer);
	laserSound->setVolume(WEAPON_VOLUME + 20);

	clickBuffer = new sf::SoundBuffer;
	clickBuffer->loadFromFile("audio/clickCCA.wav");
	clickSound = new sf::Sound;
	clickSound->setBuffer(*clickBuffer);
	clickSound->setVolume(WEAPON_VOLUME);

	coinBuffer = new sf::SoundBuffer;
	coinBuffer->loadFromFile("audio/coin.wav");
	coinSound = new sf::Sound;
	coinSound->setBuffer(*coinBuffer);
	coinSound->setVolume(WEAPON_VOLUME + 90);

	dropgunBuffer = new sf::SoundBuffer;
	dropgunBuffer->loadFromFile("audio/dropgun.wav");
	dropgunSound = new sf::Sound;
	dropgunSound->setBuffer(*dropgunBuffer);
	dropgunSound->setVolume(WEAPON_VOLUME);

	footstepBuffer1 = new sf::SoundBuffer;
	footstepBuffer2 = new sf::SoundBuffer;
	footstepBuffer3 = new sf::SoundBuffer;
	footstepBuffer1->loadFromFile("audio/step1.ogg");
	footstepBuffer2->loadFromFile("audio/step2.ogg");
	footstepBuffer3->loadFromFile("audio/step3.ogg");
	footstep1 = new sf::Sound(*footstepBuffer1);
	footstep2 = new sf::Sound(*footstepBuffer2);
	footstep3 = new sf::Sound(*footstepBuffer3);
	footstep1->setVolume(WEAPON_VOLUME);
	footstep2->setVolume(WEAPON_VOLUME);
	footstep3->setVolume(WEAPON_VOLUME);
	footsteps.push_back(footstep1);
	footsteps.push_back(footstep2);
	footsteps.push_back(footstep3);

	teleportBuffer = new sf::SoundBuffer;
	teleportBuffer->loadFromFile("audio/teleport2.ogg");
	teleportSound = new sf::Sound;
	teleportSound->setBuffer(*teleportBuffer);
	teleportSound->setVolume(WEAPON_VOLUME + 90);
}

Audio::~Audio()
{
	delete bulletSound;
	delete sniperSound;
	delete shotgunSound;
	delete laserSound;
	delete coinSound;
	delete clickSound;
	delete dropgunSound;
	delete footstep1;
	delete footstep2;
	delete footstep3;
	delete teleportSound;

	/*delete bulletBuffer;
	delete shotgunBuffer;
	delete laserBuffer;
	delete coinBuffer;
	delete clickBuffer;
	delete dropgunBuffer;
	delete footstepBuffer1;
	delete footstepBuffer2;
	delete footstepBuffer3;
	delete teleportBuffer;*/
}

void Audio::playMusic()
{
}
