#pragma once

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
