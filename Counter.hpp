#pragma once
#include <stdint.h>

struct Counter
{
	uint8_t value = 0;
	bool in = 0;
	bool out = 0;
	bool enable = 0;
};