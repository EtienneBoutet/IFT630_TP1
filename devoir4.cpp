#include <iostream>
#include "Canal.h"
#include "CartConstants.h"

using namespace std;

void cart(Receiver<int> receiver) {
	for (unsigned int i = 0; i < floor(IN_LINE_COUNT / RIDE_CAPACITY); ++i)
	{
		size_t cartCount = 0;
		while (cartCount != RIDE_CAPACITY) {
			receiver.receive();
			cartCount++;
		}
		for (int i = 0; i < LAP_COUNT; ++i) {
			cout << "WOOHOO!!!" << endl;
			std::this_thread::sleep_for(LAP_TIME);
		}
		cout << "Ride is finished." << endl;
	}
}

void consumer(unsigned int i, Sender<int> sender) {
	sender.Send(i);
}

int main(int argc, char* argv[]) {
	Canal<int> canal(RIDE_CAPACITY);
	Receiver<int> receiver(&canal);
	Sender<int> sender(&canal);

	std::vector<std::thread> threads;

	threads.push_back(std::thread(cart, receiver));

	for (unsigned int i = 0; i < IN_LINE_COUNT; ++i)
	{
		threads.push_back(std::thread(consumer, i, sender));
	}

	for (auto& thread : threads)
	{
		thread.join();
	}

	return 0;
}