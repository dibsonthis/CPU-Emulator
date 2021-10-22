#pragma once

#include <iostream>
#include <map>
#include "Bus.hpp"
#include "Register.hpp"
#include "ALU.hpp"
#include "Clock.hpp"
#include "Counter.hpp"
#include "RAM.hpp"

#define PC_IN			0b100000000000000000000000
#define PC_OUT			0b010000000000000000000000
#define AR_IN			0b001000000000000000000000
#define AR_OUT			0b000100000000000000000000
#define IR_IN			0b000010000000000000000000
#define IR_OUT			0b000001000000000000000000
#define A_IN			0b000000100000000000000000
#define A_OUT			0b000000010000000000000000
#define B_IN			0b000000001000000000000000
#define B_OUT			0b000000000100000000000000
#define RAM_IN			0b000000000010000000000000
#define RAM_OUT			0b000000000001000000000000
#define ALU_OUT			0b000000000000100000000000
#define ADD				0b000000000000010000000000
#define PC_INC			0b000000000000001000000000
#define OUT_IN			0b000000000000000100000000
#define OUT_OUT			0b000000000000000010000000
#define OUT_ASCII		0b000000000000000001000000
#define HLT				0b000000000000000000000001

#define ZERO_FLAG	0b00000001
#define POS_FLAG	0b00000010
#define NEG_FLAG	0b00000100

uint8_t flags = 0;

void set_flag(uint8_t flag, bool state)
{
	if (state == 0)
		flags = flags & ~flag;
	else
		flags = flags | flag;
}

bool get_flag(uint8_t flag)
{
	return flags & flag;
}

uint32_t control_command = 0;

Clock clk;

Bus bus;

Register_8_Bit r_a;
Register_8_Bit r_b;

Register_8_Bit address_register;
Register_8_Bit instruction_register;
Register_8_Bit output_register;

ALU_8_Bit ALU;

RAM ram;

Counter program_counter;
Counter micro_instruction_counter;

int debug = 0;

void update_bus()
{
	if (r_a.out) bus.value = r_a.value;
	if (r_b.out) bus.value = r_b.value;
	if (address_register.out) bus.value = address_register.value;
	if (instruction_register.out) bus.value = instruction_register.value;
	if (ALU.out) bus.value = ALU.value;
	if (ram.out) bus.value = ram.current;
	if (program_counter.out) bus.value = program_counter.value;
	if (program_counter.out) bus.value = program_counter.value;
}

std::map<uint32_t, uint32_t> instructions =
{
	// LDA - Z Flag 0													INS	 STP  Flags
	{ 0b000000010000001000000000, PC_OUT | AR_IN },				//0x	01   02	  00
	{ 0b000000010000001100000000, RAM_OUT | AR_IN },			//0x	01   03   00
	{ 0b000000010000010000000000, RAM_OUT | A_IN | PC_INC },	//0x	01   04	  00
	// LDB - Z Flag 0
	{ 0b000000100000001000000000, PC_OUT | AR_IN },				//0x	02   02	  00
	{ 0b000000100000001100000000, RAM_OUT | AR_IN },			//0x	02   03   00
	{ 0b000000100000010000000000, RAM_OUT | B_IN | PC_INC },	//0x	02   04	  00
	//ADD - Z Flag 0	 
	{ 0b000000110000001000000000, ADD },						//0x	03   02	  00
	{ 0b000000110000001100000000, ALU_OUT | A_IN },				//0x	03   03   00
	//STA - Z Flag 0											
	{ 0b000001000000001000000000, PC_OUT | AR_IN },				//0x	04   02	  00
	{ 0b000001000000001100000000, RAM_OUT | AR_IN },			//0x	04   03   00
	{ 0b000001000000010000000000, A_OUT | RAM_IN | PC_INC },	//0x	04   04	  00
	//JNZ - Z Flag 0	 
	{ 0b000001010000001000000000, PC_OUT | AR_IN },				//0x	05   02	  00
	{ 0b000001010000001100000000, RAM_OUT | PC_IN },			//0x	05   03   00
	//JPZ - Z Flag 0	 
	{ 0b000001100000001000000000, PC_INC | AR_IN },				//0x	06   02	  00
	//LDO - Z Flag 0	 
	{ 0b000010000000001000000000, A_OUT | OUT_IN },				//0x	08   02	  00
	//OUT - Z Flag 0	 
	{ 0b000010010000001000000000, OUT_OUT },					//0x	09   02	  00
	//OUT ASCII - Z Flag 0										
	{ 0b000010100000001000000000, OUT_ASCII | OUT_OUT },		//0x	0a   02	  00
	//LDAI - Z Flag 0	
	{ 0b000010110000001000000000, PC_OUT | AR_IN },				//0x	0b   02	  00
	{ 0b000010110000001100000000, RAM_OUT | A_IN | PC_INC },	//0x	0b   03   00
	//LDBI - Z Flag 0	
	{ 0b000011000000001000000000, PC_OUT | AR_IN },				//0x	0c   02	  00
	{ 0b000011000000001100000000, RAM_OUT | B_IN | PC_INC},		//0x	0c   03   00
	//HLT - Z Flag 0	 
	{ 0b111111110000001000000000, HLT },						//0x	ff   02	  00

	// LDA - Z Flag 1
	{ 0b000000010000001000000001, PC_OUT | AR_IN },				//0x	01   02	  01
	{ 0b000000010000001100000001, RAM_OUT | AR_IN },			//0x	01   03   01
	{ 0b000000010000010000000001, RAM_OUT | A_IN | PC_INC },	//0x	01   04	  01
	// LDB - Z Flag 1											
	{ 0b000000100000001000000001, PC_OUT | AR_IN },				//0x	02   02	  01
	{ 0b000000100000001100000001, RAM_OUT | AR_IN },			//0x	02   03   01
	{ 0b000000100000010000000001, RAM_OUT | B_IN | PC_INC },	//0x	02   04	  01
	//ADD - Z Flag 1	 										
	{ 0b000000110000001000000001, ADD },						//0x	03   02	  01
	{ 0b000000110000001100000001, ALU_OUT | A_IN },				//0x	03   03   01
	//STA - Z Flag 1											
	{ 0b000001000000001000000001, PC_OUT | AR_IN },				//0x	04   02	  01
	{ 0b000001000000001100000001, RAM_OUT | AR_IN },			//0x	04   03   01
	{ 0b000001000000010000000001, A_OUT | RAM_IN | PC_INC },	//0x	04   04	  01
	//JNZ - Z Flag 1	 										
	{ 0b000001010000001000000001, PC_INC },						//0x	05   02	  01
	//JPZ - Z Flag 1	 										
	{ 0b000001100000001000000001, PC_OUT | AR_IN },				//0x	06   02	  01
	{ 0b000001100000001100000001, RAM_OUT | AR_IN },			//0x	06   03   01
	//LDO - Z Flag 1	 										
	{ 0b000010000000001000000001, A_OUT | OUT_IN },				//0x	08   02	  01
	//OUT - Z Flag 1	 										
	{ 0b000010010000001000000001, OUT_OUT },					//0x	09   02	  01
	//OUT ASCII - Z Flag 1	 										
	{ 0b000010100000001000000001, OUT_ASCII | OUT_OUT },		//0x	0a   02	  01
	//LDAI - Z Flag 0	
	{ 0b000010110000001000000001, PC_OUT | AR_IN },				//0x	0b   02	  01
	{ 0b000010110000001100000001, RAM_OUT | A_IN | PC_INC },	//0x	0b   03   01
	//LDBI - Z Flag 0	
	{ 0b000011000000001000000001, PC_OUT | AR_IN },				//0x	0c   02	  01
	{ 0b000011000000001100000001, RAM_OUT | B_IN | PC_INC },	//0x	0c   03   01
	//HLT - Z Flag 1	 										
	{ 0b111111110000001000000001, HLT }							//0x	ff   02	  01
};


void decode_instruction(uint8_t instruction, uint8_t& mi_step)
{
	// Fetch Instructions //

	if (mi_step == 0)
	{
		control_command = PC_OUT | AR_IN;

		if (debug)
		{
			std::cout << "\nFetch (1/2)";
		}
	}

	else if (mi_step == 1)
	{
		control_command = RAM_OUT | IR_IN | PC_INC;

		if (debug)
		{
			std::cout << "\nFetch (2/2)";
		}
	}

	// ----------------- //

	else
	{
		uint32_t instruction_index = (instruction << 16) | (mi_step << 8) | flags;
		control_command = instructions[instruction_index];

		if (debug)
		{
			std::cout << "\nProgram Counter: " << (int)program_counter.value << " - Instruction: " << (int)instruction << " - Step: " << (int)mi_step << " - Flags: " << (int)flags;
		}
	}

	program_counter.enable = control_command & PC_INC;
	program_counter.out = control_command & PC_OUT;
	instruction_register.out = control_command & IR_OUT;
	address_register.out = control_command & AR_OUT;
	r_a.out = control_command & A_OUT;
	r_b.out = control_command & B_OUT;
	ram.out = control_command & RAM_OUT;
	ALU.out = control_command & ALU_OUT;
	output_register.out = control_command & OUT_OUT;
	output_register.ascii = control_command & OUT_ASCII;

	program_counter.in = control_command & PC_IN;
	instruction_register.in = control_command & IR_IN;
	address_register.in = control_command & AR_IN;
	r_a.in = control_command & A_IN;
	r_b.in = control_command & B_IN;
	ram.in = control_command & RAM_IN;
	ALU.add = control_command & ADD;
	output_register.in = control_command & OUT_IN;

	clk.halt = control_command & HLT;
}

void update_components()
{
	if (program_counter.enable)
	{
		program_counter.value++;
	}
	if (program_counter.in) program_counter.value = bus.value;
	if (r_a.in) r_a.value = bus.value;
	if (r_b.in) r_b.value = bus.value;
	if (output_register.in) output_register.value = bus.value;
	if (address_register.in)
	{
		address_register.value = bus.value;
		ram.current = ram.address[address_register.value];
	}
	if (instruction_register.in) instruction_register.value = bus.value;
	if (ALU.in) ALU.value = bus.value;
	if (ALU.add)
	{
		add_8_bit(ALU, r_a.value, r_b.value);
		if (ALU.value == 0)
		{
			set_flag(ZERO_FLAG, 1);
		}
		else
		{
			set_flag(ZERO_FLAG, 0);
		}
	}
	if (ram.in)
	{
		ram.current = bus.value;
		ram.address[address_register.value] = bus.value;
	}
	if (output_register.out)
	{
		if (output_register.ascii)
		{
			std::cout << output_register.value;
		}
		else
		{
			std::cout << (int)output_register.value;
		}
	}
}

/*

LDA			0b00000001	0x01
LDB			0b00000010	0x02
ADD			0b00000011	0x03
STA			0b00000100	0x04
JNZ			0b00000101	0x05
JPZ			0b00000110	0x06
LDO			0b00001000	0x08
OUT			0b00001001	0x09
OUT_ASCII	0b00001010	0x0a
LDA_IMM		0b00001011	0x0b
LDB_IMM		0b00001100	0x0c
HLT			0b11111111	0xff

*/

void run()
{
	clk.speed = 100000;

	/* Instructions */

	//ram.address[0] = 0x0b; // LDAI
	//ram.address[1] = 0xfd; // $253

	//ram.address[2] = 0x0c; // LDBI
	//ram.address[3] = 0x01; // $1

	//ram.address[4] = 0x03; // ADD

	//ram.address[5] = 0x04; // STA
	//ram.address[6] = 0xcb; // 203

	//ram.address[7] = 0x08; // LDO

	//ram.address[8] = 0x09; // OUT

	//ram.address[9] = 0x0b; // LDAI
	//ram.address[10] = 0x20; // $32 == ' '

	//ram.address[11] = 0x08; // LDO
	//ram.address[12] = 0x0a; // OUT_ASCII

	//ram.address[13] = 0x01; // LDA
	//ram.address[14] = 0xcb; // 203

	//ram.address[15] = 0x05; // JNZ
	//ram.address[16] = 0x04; // 4

	//ram.address[17] = 0xff; // HLT

	uint8_t mic = 0;

	while (clk.halt != true)
	{
		pulse(clk);
		decode_instruction(instruction_register.value, mic);
		update_bus();
		update_components();

		if (mic < 5)
			mic++;
		else
			mic = 0;
	}

	std::cout << "\n\nDone.";
	std::cout << "\n\nHex Dump:\n";
	hex_dump(ram);
	std::cout << "\n\nBin Dump:\n";
	bin_dump(ram);
	std::cin.get();
}