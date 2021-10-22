#pragma once
#include <map>
#include "Lexer.hpp"

#define INST_LDA	0x01
#define INST_LDB	0x02
#define INST_ADD	0x03
#define INST_STA	0x04
#define INST_JNZ	0x05
#define INST_JPZ	0x06
#define INST_LDO	0x08
#define INST_OUT	0x09
#define INST_OUT_C	0x0a
#define INST_LDAI	0x0b
#define INST_LDBI	0x0c
#define INST_HLT	0xff

struct ParserEnvironment
{
	Token* token;
	int index = 0;
	std::string file_name;
	int line;
	int column;
	std::vector<Token*> tokens;
	uint8_t address[256] = { 0 };
	int current_address = 0;
	std::map<std::string, int> labels;
};

class Parser
{
public:

	ParserEnvironment* new_parser_env(LexerEnvironment* lexer_env);

	void parse(ParserEnvironment* p_env);

private:

	void error_and_exit(ParserEnvironment* p_env, std::string message);

	void advance(ParserEnvironment* p_env);
};