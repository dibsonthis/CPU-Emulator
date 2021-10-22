#pragma once
#include <stdint.h>
#include <string>
#include <vector>
#include <fstream>
#include <streambuf>

#include "Type.hpp"

std::string load_source(std::string filename);

struct Token
{
	Type type = TYPE_EMPTY;
	int line;
	int column;

	union
	{
		char char_value;
		int32_t int_value;
		float float_value;
		long hex_value;
		std::string* string_value;
		std::string* id_name;
	};

	bool op = false;
	bool unary_op = false;
	bool literal_type = false;
};

struct LexerEnvironment
{
	std::string file_name;
	std::string source;
	int source_length;
	int index = 0;
	char current_char;
	int line = 1;
	int column = 1;
	std::vector<Token*> tokens;
};

class Lexer
{
public:
	
	std::vector<LexerEnvironment*> lexer_env;
	LexerEnvironment* new_lexer_env(std::string filename);
	void advance(LexerEnvironment* lexer_env, int n);
	char peek(LexerEnvironment* lexer_env, int n);
	void build_word(LexerEnvironment* lexer_env);
	void build_number(LexerEnvironment* lexer_env);
	void build_string(LexerEnvironment* lexer_env);
	void build_char(LexerEnvironment* lexer_env);
	void add_token(LexerEnvironment* lexer_env, Type type, int size, int op, int unary_op);
	void handle_line_comment(LexerEnvironment* lexer_env);
	void handle_block_comment(LexerEnvironment* lexer_env);
	void tokenize(LexerEnvironment* lexer_env);
	void print_tokens(LexerEnvironment* lexer_env);
	void error_and_exit(LexerEnvironment* lexer_env, std::string message);
};