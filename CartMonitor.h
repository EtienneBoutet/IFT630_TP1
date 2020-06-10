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
	CartMonitor(std::mutex* mtx) {
		canEnter = new Semaphore(0);
		consumerEntered = new Semaphore(0);
		cartThread = std::thread(&CartMonitor::cart, this);
		printMtx = mtx;
	}

	~CartMonitor() {
		delete canEnter;
		delete consumerEntered;
		cartThread.join();
	}

	void enter() {
		canEnter->P();
		cartCount++;
		consumerEntered->V();
	}
private:
	Semaphore* canEnter;
	Semaphore* consumerEntered;
	size_t cartCount = 0;
	std::thread cartThread;
	std::mutex* printMtx;

	void cart() {
		for (unsigned int i = 0; i < floor(IN_LINE_COUNT / RIDE_CAPACITY); ++i)
		{
			while (cartCount != RIDE_CAPACITY) {
				canEnter->V();
				consumerEntered->P();
			}
			for (int i = 0; i < LAP_COUNT; ++i) {
				printMtx->lock();
				cout << "WOOHOO!!!" << endl;
				printMtx->unlock();
				std::this_thread::sleep_for(LAP_TIME);
			}
			printMtx->lock();
			cout << "Ride is finished" << endl;
			printMtx->unlock();
			cartCount = 0;
		}
	}
};