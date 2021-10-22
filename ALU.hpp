#pragma once
#include <stdint.h>

struct ALU_8_Bit
{
	uint8_t value = 0;
	bool in = 0;
	bool out = 0;
	bool add = 0;
	bool sub = 0;
	bool mul = 0;
	bool div = 0;
};

void add_8_bit(ALU_8_Bit& ALU, uint8_t a, uint8_t b);

void sub_8_bit(ALU_8_Bit& ALU, uint8_t a, uint8_t b);

void mul_8_bit(ALU_8_Bit& ALU, uint8_t a, uint8_t b);

void div_8_bit(ALU_8_Bit& ALU, uint8_t a, uint8_t b);

void cmp_8_bit(ALU_8_Bit& ALU, uint8_t a, uint8_t b);