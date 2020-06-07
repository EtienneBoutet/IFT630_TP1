#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <random>
#include <time.h>

#include "semaphore.h"

using namespace std;

enum class State {
	THINKING,
	HUNGRY,
	EATING
};

class Philosopher {
public:
	State state = State::THINKING;
	int eatCount = 0;
};

const std::chrono::seconds ACTION_TIME = 1s;
const unsigned int THINK_THRESHOLD = 50;
const unsigned int TIMES_TO_EAT = 1;
Semaphore* requests[5];
Philosopher* philosophers[5];
std::mutex mtx;

int intRand() {
	std::hash<std::thread::id> hasher;
	static thread_local mt19937* generator = nullptr;
	if (!generator) generator = new mt19937(clock() + hasher(this_thread::get_id()));
	uniform_int_distribution<int> distribution(0, 100);
	return distribution(*generator);
}

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
		default:
			break;
		}
		std::this_thread::sleep_for(ACTION_TIME);
	}
}

int main(int argc, char* argv[])
{
	for (unsigned int i = 0; i < 5; ++i)
	{
		philosophers[i] = new Philosopher();
		requests[i] = new Semaphore(0);
	}

	std::vector<std::thread> threads;

	for (unsigned int i = 0; i < 5; ++i)
	{
		threads.push_back(std::thread(live, i));
	}

	for (auto &thread: threads)
	{
		thread.join();
	}

	cout << "Everyone's belly is full" << endl;
	return 0;
}
