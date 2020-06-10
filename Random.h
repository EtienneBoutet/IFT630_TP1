#pragma once

#include <random>
#include <time.h>

using namespace std;

unsigned int intRand() {
	std::hash<std::thread::id> hasher;
	static thread_local mt19937* generator = nullptr;
	if (!generator) generator = new mt19937(clock() + hasher(this_thread::get_id()));
	uniform_int_distribution<int> distribution(0, 100);
	return distribution(*generator);
}