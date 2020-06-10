#pragma once

enum class State {
	THINKING,
	HUNGRY,
	EATING
};

struct Philosopher {
	State state = State::THINKING;
	int eatCount = 0;
};