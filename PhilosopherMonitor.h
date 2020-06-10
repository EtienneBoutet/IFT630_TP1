#pragma once

#include "Random.h"

class PhilosopherMonitor {
public:
	PhilosopherMonitor() {
		for (unsigned int i = 0; i < 5; ++i)
		{
			philosophers[i] = new Philosopher();
		}
	}

	~PhilosopherMonitor() {
		for (unsigned int i = 0; i < 5; ++i)
		{
			delete philosophers[i];
		}
	}

	Philosopher* get(unsigned int i) {
		return philosophers[i];
	}

	void startEating(unsigned int i) {
		std::unique_lock<std::mutex> lock(mtx);
		philosophers[i]->state = State::HUNGRY;
		tryEat(i);
		if (philosophers[i]->state != State::EATING) wait[i].wait(lock);
	}

	void stopEating(unsigned int i) {
		unique_lock<std::mutex> lock(mtx);
		philosophers[i]->eatCount++;
		philosophers[i]->state = State::THINKING;
		tryEat((i + 4) % 5);
		tryEat((i + 1) % 5);
	}

private:
	condition_variable wait[5];
	Philosopher* philosophers[5];
	std::mutex mtx;

	void tryEat(unsigned int i) {
		if (philosophers[(i + 1) % 5]->state != State::EATING
			&& philosophers[(i + 4) % 5]->state != State::EATING
			&& philosophers[i]->state == State::HUNGRY) {
			philosophers[i]->state = State::EATING;
			wait[i].notify_one();
		}
	}
};