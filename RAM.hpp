#pragma once
#include <iostream>
#include <stdint.h>
#include <bitset>
#include <iomanip>

struct RAM
{
	uint8_t address[256] = { 0 };
	uint8_t current = address[0];
	bool in = 0;
	bool out = 0;
};

void hex_dump(RAM& ram);

void bin_dump(RAM& ram);