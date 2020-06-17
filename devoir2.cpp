#include <iostream>
#include <thread>
#include <mutex>

#include "semaphore.h"
#include "CartMonitor.h"
#include "CartSemaphore.h"

using namespace std;

void monitorLifecycle(unsigned int i, CartMonitor* monitor) {
	monitor->enter(i);
}

int main(int argc, char* argv[]) {
	bool useMonitor = false;
	for (int i = 0; i < argc; ++i)
		if (!strcmp(argv[i], "-sw")) useMonitor = true;
	if (useMonitor) {
		cout << "Using monitor" << endl;
		CartMonitor* monitor = new CartMonitor();
		std::vector<std::thread> threads;

		for (unsigned int i = 0; i < IN_LINE_COUNT; ++i)
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
		CartSemaphore* semaphore = new CartSemaphore();
		semaphore->exec();
		delete semaphore;
	}	
	cout << "Everyone had fun." << endl;
	return 0;
}
