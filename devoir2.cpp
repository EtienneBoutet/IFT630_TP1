#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <math.h>

#include "semaphore.h"

using namespace std;

const std::chrono::seconds LAP_TIME = 1s;
const unsigned int LAP_COUNT = 5;
const unsigned int RIDE_CAPACITY = 15;
const unsigned int IN_LINE_COUNT = 60;
Semaphore canEnter(0);
Semaphore consumerEntered(0);
size_t cartCount = 0;

void cart() {
	for (unsigned int i = 0; i < floor(IN_LINE_COUNT / RIDE_CAPACITY); ++i)
	{
		while (cartCount != RIDE_CAPACITY) {
			canEnter.V();
			consumerEntered.P();
		}
		for (int i = 0; i < LAP_COUNT; ++i) {
			cout << "WOOHOO!!!" << endl;
			std::this_thread::sleep_for(LAP_TIME);
		}
		cout << "Ride is finished" << endl;
		cartCount = 0;
	}
}

void consumer(unsigned int i) {
	canEnter.P();
	cartCount++;
	cout << "Consumer " << i << "   entered the ride" << endl;
	consumerEntered.V();
}

int main(int argc, char* argv[])
{
	std::vector<std::thread> threads;

	for (unsigned int i = 1; i <= IN_LINE_COUNT; ++i)
	{
		threads.push_back(std::thread(consumer, i));
	}

	threads.push_back(std::thread(cart));

	for (auto &thread: threads)
	{
		thread.join();
	}

	cout << "Everyone had fun." << endl;
	return 0;
}
