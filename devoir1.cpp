#include <iostream>
#include <thread>
#include <vector>
#include <mutex>

#include "semaphore.h"
#include "philosopher.h"

using namespace std;

const std::chrono::seconds ACTION_TIME = 1s;
const unsigned int THINK_THRESHOLD = 80;
const unsigned int TIMES_TO_EAT = 5;
std::mutex mtx;
Semaphore* requests[5];
Philosopher* philosophers[5];

void tryEat(unsigned int i) {
	if (philosophers[(i + 1) % 5]->state != State::EATING
		&& philosophers[(i + 4) % 5]->state != State::EATING
		&& philosophers[i]->state == State::HUNGRY) {
		cout << "Philosopher: " << i << "   Started eating" << endl;
		philosophers[i]->state = State::EATING;
		requests[i]->V();
	}
}

void live(unsigned int i) {
	Philosopher* philosopher = philosophers[i];
	while (philosopher->eatCount != TIMES_TO_EAT)
	{
		// Finished eating
		if (philosopher->state == State::EATING) {
			mtx.lock();
			philosopher->eatCount++;
			philosopher->state = State::THINKING;
			cout << "Philosopher: " << i << "   Stopped eating" << endl;
			tryEat((i + 4) % 5);
			tryEat((i + 1) % 5);
			mtx.unlock();
		}
		else {
			// Gets hungry and tries to eat
			if ((int)(rand() % 100 ) >= THINK_THRESHOLD) {
				mtx.lock();
				cout << "Philosopher: " << i << "   Got hungry" << endl;
				philosopher->state = State::HUNGRY;
				tryEat(i);
				mtx.unlock();
				requests[i]->P();
			}
		}
		std::this_thread::sleep_for(ACTION_TIME);
	}
}

int main(int argc, char* argv[])
{
	// Seed random
	srand((unsigned)time(NULL));

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

	cout << "Everyone's belly is full." << endl;
	return 0;
}
