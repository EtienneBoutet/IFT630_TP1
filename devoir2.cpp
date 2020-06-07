#include <iostream>
#include <thread>
#include <vector>
#include <mutex>

#include "semaphore.h"

using namespace std;

const std::chrono::seconds LAP_TIME = 1s;
const unsigned int LAP_COUNT = 5;
const unsigned int RIDE_CAPACITY = 15;
const unsigned int IN_LINE_COUNT = 60;
const unsigned int TIMES_TO_EAT = 5;
Semaphore canEnter(0);
Semaphore consumerEntered(0);
std::vector<int> cart;

void cartFn() {
	while (true) {
		while (cart.size() != RIDE_CAPACITY) {
			canEnter.V();
			consumerEntered.P();
		}
		for (int i = 0; i < LAP_COUNT; ++i) {
			cout << "WOOHOO!!!" << endl;
			std::this_thread::sleep_for(LAP_TIME);
		}
		cout << "Ride is finished" << endl;
		cart.clear();
	}
}

void consumer(unsigned int i) {
	canEnter.P();
	cart.push_back(i);
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

	threads.push_back(std::thread(cartFn));

	for (auto &thread: threads)
	{
		thread.join();
	}

	cout << "Everyone had fun." << endl;
	return 0;
}
