#pragma once

#include <thread>
#include <math.h>
#include <vector>
#include <iostream>

#include "semaphore.h"
#include "CartConstants.h"

using namespace std;

class CartMonitor {
public:
	CartMonitor() {
		canEnter = new Semaphore(0);
		canExit = new Semaphore(0);
		consumerEntered = new Semaphore(0);
		consumerExited = new Semaphore(0);
		cartThread = std::thread(&CartMonitor::cart, this);
	}

	~CartMonitor() {
		delete canEnter;
		delete canExit;
		delete consumerEntered;
		delete consumerExited;
		cartThread.join();
	}

	void enter(unsigned int i) {
		canEnter->P();
		cout << "Consumer " << i << "   entered the ride" << endl;
		consumerEntered->V();
		canExit->P();
		cout << "Consumer " << i << "   exited the ride" << endl;
		consumerExited->V();
	}
private:
	Semaphore* canEnter;
	Semaphore* canExit;
	Semaphore* consumerEntered;
	Semaphore* consumerExited;
	size_t cartCount = 0;
	std::thread cartThread;

	void cart() {
		for (unsigned int i = 0; i < floor(IN_LINE_COUNT / RIDE_CAPACITY); ++i)
		{
			while (cartCount != RIDE_CAPACITY) {
				canEnter->V();
				consumerEntered->P();
				cartCount++;
			}
			for (int i = 0; i < LAP_COUNT; ++i) {
				cout << "WOOHOO!!!" << endl;
				std::this_thread::sleep_for(LAP_TIME);
			}
			cout << "Ride is finished" << endl;
			while (cartCount != 0) {
				canExit->V();
				consumerExited->P();
				cartCount--;
			}
		}
	}
};