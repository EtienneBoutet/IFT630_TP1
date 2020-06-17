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
		consumerEntered = new Semaphore(0);
		cartThread = std::thread(&CartMonitor::cart, this);
	}

	~CartMonitor() {
		delete canEnter;
		delete consumerEntered;
		cartThread.join();
	}

	void enter(unsigned int i) {
		canEnter->P();
		mtx.lock();
		cout << "Consumer " << i << "   entered the ride" << endl;
		consumerEntered->V();
		mtx.unlock();
	}
private:
	Semaphore* canEnter;
	Semaphore* consumerEntered;
	size_t cartCount = 0;
	std::thread cartThread;
	std::mutex mtx;

	void cart() {
		for (unsigned int i = 0; i < floor(IN_LINE_COUNT / RIDE_CAPACITY); ++i)
		{
			while (cartCount != RIDE_CAPACITY) {
				canEnter->V();
				consumerEntered->P();
				cartCount++;
			}
			for (int i = 0; i < LAP_COUNT; ++i) {
				mtx.lock();
				cout << "WOOHOO!!!" << endl;
				mtx.unlock();
				std::this_thread::sleep_for(LAP_TIME);
			}
			mtx.lock();
			cout << "Ride is finished" << endl;
			cartCount = 0;
			mtx.unlock();
		}
	}
};