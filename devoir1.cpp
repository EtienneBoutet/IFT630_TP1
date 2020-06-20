#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <stdio.h>
#include <string.h>

#include "semaphore.h"
#include "Philosopher.h"
#include "PhilosopherSemaphore.h"
#include "PhilosopherMonitor.h"
#include "PhilosopherConstants.h"

using namespace std;

std::mutex printMtx;

void monitorLifecycle(unsigned int i, PhilosopherMonitor* monitor) {
	Philosopher* philosopher = monitor->get(i);
	while (philosopher->eatCount != TIMES_TO_EAT)
	{
		switch (philosopher->state)
		{
		case State::EATING:
			printMtx.lock();
			cout << "Philosopher " << i << " state: eating" << endl;
			printMtx.unlock();
			monitor->stopEating(i);
			break;
		case State::THINKING:
			printMtx.lock();
			cout << "Philosopher " << i << " state: thinking" << endl;
			printMtx.unlock();
			if (intRand() >= THINK_THRESHOLD) {
				printMtx.lock();
				cout << "Philosopher " << i << " state: hungry" << endl;
				printMtx.unlock();
				monitor->startEating(i);
			}
			break;
		}
		std::this_thread::sleep_for(ACTION_TIME);
	}
	printMtx.lock();
	cout << "Philosopher " << i << " state: done" << endl;
	printMtx.unlock();
}

int main(int argc, char* argv[]) {
	bool useMonitor = false;
	for (int i = 0; i < argc; ++i)
		if (!strcmp(argv[i], "-sw")) useMonitor = true;
	if (useMonitor) {
		cout << "Using monitor" << endl;
		PhilosopherMonitor* monitor = new PhilosopherMonitor();
		std::vector<std::thread> threads;

		for (unsigned int i = 0; i < 5; ++i)
		{
			threads.push_back(std::thread(monitorLifecycle, i, monitor));
		}

		for (auto& thread : threads)
		{
			thread.join();
		}
		delete monitor;
	} else {
		cout << "Using semaphores" << endl;
		PhilosopherSemaphore* semaphore = new PhilosopherSemaphore();
		semaphore->exec();
		delete semaphore;
	}
	cout << "Everyone's belly is full" << endl;
	return 0;
}
