#pragma once
#include <iostream>
#include <chrono>
#include <thread>

enum ClockState
{
	LOW,
	HIGH
};

struct Clock
{
	bool state = LOW;
	int speed = 10000; // TODO: not sure if this is how we want to represent this yet
	bool manual = 0;
	bool halt = 0;
};

void pulse(Clock clk);