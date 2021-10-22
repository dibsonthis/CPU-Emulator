#pragma once
#include <stdint.h>

struct Register_8_Bit
{
	uint8_t value = 0;
	bool in = 0;
	bool out = 0;
	bool ascii = 0;
};

struct Register_16_Bit
{
	uint16_t value = 0;
};

struct Register_32_Bit
{
	uint32_t value = 0;
};