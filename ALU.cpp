#include "ALU.hpp"

void add_8_bit(ALU_8_Bit& ALU, uint8_t a, uint8_t b)
{
	ALU.value = a + b;
}

void sub_8_bit(ALU_8_Bit& ALU, uint8_t a, uint8_t b)
{
	ALU.value = a - b;
}

void mul_8_bit(ALU_8_Bit& ALU, uint8_t a, uint8_t b)
{
	ALU.value = a * b;
}

void div_8_bit(ALU_8_Bit& ALU, uint8_t a, uint8_t b)
{
	ALU.value = a / b;
}

void cmp_8_bit(ALU_8_Bit& ALU, uint8_t a, uint8_t b)
{
	if (a == b)
	{
		ALU.value = 0;
	}
	else if (a < b)
	{
		ALU.value = -1;
	}
	if (a > b)
	{
		ALU.value = 1;
	}
}