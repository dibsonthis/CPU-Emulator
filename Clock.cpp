#include "Clock.hpp"

void pulse(Clock clk)
{
	clk.state = HIGH;
	std::this_thread::sleep_for(std::chrono::nanoseconds(clk.speed));
	clk.state = LOW;
}
