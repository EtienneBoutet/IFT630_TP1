#include <queue>
#include "semaphore.h"

template <class T>
class Canal {
public:
	Canal(size_t size) {
		items = new Semaphore(size);
		count = new Semaphore(0);
	}

	~Canal() {
		delete items;
		delete count;
	}

	void addToQueue(T push) {
		items->P();
		buffer.push(push);
		count->V();
	}

	T removeFromQueue() {
		count->P();
		T pop = buffer.front();
		buffer.pop();
		items->V();
		return pop;
	}

private:
	std::queue<T> buffer;
	Semaphore* items;
	Semaphore* count;
};

template <class T>
class Receiver {
public:
	Receiver(Canal<T>* canal) {
		this->canal = canal;
	}
	T receive() {
		return canal->removeFromQueue();
	}
private:
	Canal<T>* canal;
};

template <class T>
class Sender {
public:
	Sender(Canal<T>* canal) {
		this->canal = canal;
	}
	void Send(T push) {
		return canal->addToQueue(push);
	}
private:
	Canal<T>* canal;
};