#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <sstream>

#include "semaphore.h"

using namespace std;

const unsigned int CONSUMER_COUNT = 10;
const unsigned int WAITING_ROOM_COUNT = 3;
const chrono::seconds MOVE_TIME = 1s;
const chrono::seconds CUT_TIME = 2s;
const chrono::seconds TIME_MAX = 10s;
const chrono::seconds TIME_MIN = 1s;

Semaphore canMove(1);
Semaphore wait(1);
Semaphore openDoor(0);
Semaphore consumerArrived(0);

std::condition_variable seated;
std::condition_variable exiting;

std::mutex mtx;
std::mutex printMtx;

unsigned int cousumerCount = 0;

void print(const string a) {
	printMtx.lock();
	cout << a << endl;
	printMtx.unlock();
}

void moveBarber(const string message) {
	canMove.P();
	std::this_thread::sleep_for(MOVE_TIME);
	print(message);
	canMove.V();
}

void barber() {
	std::mutex barberMtx;
	std::unique_lock<std::mutex> lock(barberMtx);

	while (true) {
		mtx.lock();
		if (cousumerCount <= 0) {
			moveBarber("Barber goes to sleep");
			mtx.unlock();
			consumerArrived.P();
			print("Barber wakes up");
		} else mtx.unlock();

		// move to his seat when customer is sat
		seated.wait(lock);
		moveBarber("Barber moves to his chair");

		// cut hair
		print("Barber cuts hair");
		std::this_thread::sleep_for(CUT_TIME);

		// move and open door
		moveBarber("Barber moves to door");
		openDoor.V();

		// wait for customer to exit and calls next customer
		exiting.wait(lock);
		cousumerCount--;
		wait.V();
	}
}

void consumer(unsigned int i) {
	print(std::string("Consumer ") + std::to_string(i) + " arrives");

	// if no place get out
	mtx.lock();
	if (cousumerCount <= 0) consumerArrived.V();
	if (cousumerCount >= WAITING_ROOM_COUNT + 1) {
		print(std::string("Consumer ") + std::to_string(i) + " go away since waiting room is full");
		mtx.unlock();
		return;
	} else {
		cousumerCount++;
		mtx.unlock();
	};

	wait.P();

	// move to barber seat
	moveBarber(std::string("Consumer ") + std::to_string(i) + " moves to barber seat");
	seated.notify_one();

	// wait for barber to open door then exit
	openDoor.P();
	moveBarber(std::string("Consumer ") + std::to_string(i) + " moves to exit");
	exiting.notify_one();
	print(std::string("Consumer ") + std::to_string(i) + " exited");
}

int main(int argc, char* argv[]) {
	std::vector<std::thread> threads;

	threads.push_back(std::thread(barber));

	for (unsigned int i = 0; i < CONSUMER_COUNT; ++i)
	{
		threads.push_back(std::thread(consumer, i));
	}

	for (auto& thread : threads)
	{
		thread.join();
	}

	return 0;
}
