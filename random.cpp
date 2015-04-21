#include "random.hpp"

Random::Random() {
	RandomDevice rd;
	seed_ = rd();
	engine = new Engine(seed_);
}

Random::Random(int seed) {
	seed_ = seed;
	engine = new Engine(seed_);
}

Random::~Random() {
	delete engine;
}

int Random::generate(int min, int max) {
	return std::uniform_int_distribution<int>(min, max) (*engine);
}

int Random::getSeed() {
	return seed_;
}
