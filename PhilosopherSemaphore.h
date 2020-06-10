#pragma once

#include <vector>
#include <iostream>

#include "Random.h"
#include "semaphore.h"
#include "PhilosopherConstants.h"

using namespace std;

class PhilosopherSemaphore {
public:
	PhilosopherSemaphore() {
		for (unsigned int i = 0; i < 5; ++i)
		{
			philosophers[i] = new Philosopher();
			requests[i] = new Semaphore(0);
		}
	}

	~PhilosopherSemaphore() {
		for (unsigned int i = 0; i < 5; ++i)
		{
			delete philosophers[i];
			delete requests[i];
		}
	}

	void exec() {
		std::vector<std::thread> threads;

		for (unsigned int i = 0; i < 5; ++i)
		{
			threads.push_back(std::thread(&PhilosopherSemaphore::live, this, i));
		}

		for (auto& thread : threads)
		{
			thread.join();
		}

		cout << "Everyone's belly is full" << endl;
	}
private:
	Semaphore* requests[5];
	Philosopher* philosophers[5];
	std::mutex mtx;

	void tryEat(unsigned int i) {
		if (philosophers[(i + 1) % 5]->state != State::EATING
			&& philosophers[(i + 4) % 5]->state != State::EATING
			&& philosophers[i]->state == State::HUNGRY) {
			philosophers[i]->state = State::EATING;
			requests[i]->V();
		}
	}

	void live(unsigned int i) {
		Philosopher* philosopher = philosophers[i];
		while (philosopher->eatCount != TIMES_TO_EAT)
		{
			switch (philosopher->state)
			{
			case State::EATING:
				mtx.lock();
				cout << "Philosopher " << i << " state: eating" << endl;
				philosopher->eatCount++;
				philosopher->state = State::THINKING;
				tryEat((i + 4) % 5);
				tryEat((i + 1) % 5);
				mtx.unlock();
				break;
			case State::HUNGRY:
				mtx.lock();
				cout << "Philosopher " << i << " state: hungry" << endl;
				tryEat(i);
				mtx.unlock();
				requests[i]->P();
				break;
			case State::THINKING:
				mtx.lock();
				cout << "Philosopher " << i << " state: thinking" << endl;
				if (intRand() >= THINK_THRESHOLD) {
					philosopher->state = State::HUNGRY;
				}
				mtx.unlock();
				break;
			}
			std::this_thread::sleep_for(ACTION_TIME);
		}
		mtx.lock();
		cout << "Philosopher " << i << " state: done" << endl;
		mtx.unlock();
	}
};