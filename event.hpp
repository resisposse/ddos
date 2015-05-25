/*
 * Dark Domains Of Space
 * 2015 © Project Team (see: LICENSE)
 */

#ifndef EVENT
#define EVENT

class Event
{
public:
	bool mIsMovingUpSound = false;
	bool mIsMovingDownSound = false;
	bool mIsMovingLeftSound = false;
	bool mIsMovingRightSound = false;
	int random;
	float clock = 0.0;
	bool isPlaying = false;
	sf::Vector2f pos;
	float cooldown = 0;
	void processEvent();
	void playFootstepSound();
};

#endif
