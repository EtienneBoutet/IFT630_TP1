#pragma once

#include <mutex>
#include <condition_variable>

class Semaphore {
public:
	Semaphore(size_t initial_count) : count(initial_count) {}

	void V() {
		std::unique_lock<std::mutex> lock(mtx);
		count++;
		cv.notify_one();
	}

	void P() {
		std::unique_lock<std::mutex> lock(mtx);
		while (count == 0) { cv.wait(lock); }
		count--;
	}

private:
	std::mutex mtx;
	std::condition_variable cv;
	size_t count;
};
