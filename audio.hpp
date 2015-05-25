/*
 *
 */

#ifndef AUDIO
#define AUDIO

class Audio
{
public:
	sf::SoundBuffer *bulletBuffer;
	sf::Sound *bulletSound;
	sf::SoundBuffer *sniperBuffer;
	sf::Sound *sniperSound;
	sf::SoundBuffer *shotgunBuffer;
	sf::Sound *shotgunSound;
	sf::SoundBuffer *laserBuffer;
	sf::Sound *laserSound;
	sf::SoundBuffer *clickBuffer;
	sf::Sound *clickSound;
	sf::SoundBuffer *coinBuffer;
	sf::Sound *coinSound;
	sf::SoundBuffer *dropgunBuffer;
	sf::Sound *dropgunSound;
	sf::SoundBuffer *teleportBuffer;
	sf::Sound *teleportSound;

	sf::SoundBuffer *footstepBuffer1;
	sf::SoundBuffer *footstepBuffer2;
	sf::SoundBuffer *footstepBuffer3;
	sf::Sound *footstep1;
	sf::Sound *footstep2;
	sf::Sound *footstep3;
	std::vector<sf::Sound *> footsteps;
	int randomFootstep;

	Audio();
	~Audio();
	void playMusic();
};

#endif
