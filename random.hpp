
#ifndef RANDOM
#define RANDOM
#include <random>
class Random
{
public:
	Random();
	Random(int seed);
	~Random();
	int generate(int min, int max);
	int getSeed();
private:
	typedef	std::random_device RandomDevice;
	typedef std::mt19937 Engine;
	typedef std::uniform_int_distribution<int> Distribution;
	int seed_;
	Engine *engine;
};

#endif