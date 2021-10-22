#include "RAM.hpp"

void hex_dump(RAM& ram)
{
	std::cout << "\n";

	for (uint8_t byte : ram.address)
	{
		std::cout << std::showbase << std::internal << std::setfill('0');
		std::cout << std::hex << std::setw(4) << (int)byte << std::dec << "  ";
	}
}

void bin_dump(RAM& ram)
{
	std::cout << "\n";

	for (uint8_t byte : ram.address)
	{
		std::bitset<8> _byte(byte);
		std::cout << _byte << "  ";
	}
}