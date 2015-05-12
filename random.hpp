/*
 * Dark Domains Of Space
 * 2015 © Project Team (see: LICENSE)
 */

#ifndef RANDOM
#define RANDOM

#include <random>

typedef	std::random_device RandomDevice;
typedef std::mt19937 Engine;
typedef std::uniform_int_distribution<int> Distribution;

class Random
{
public:
	Random();
	Random(int seed);
	~Random();
	int generate(int min, int max);
	int generateNormal(int min, int max);
	int getSeed();
private:
	int seed_;
	Engine *engine;
};

#endif
