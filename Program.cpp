#include "CPU.hpp"
#include "Lexer.hpp"
#include "Parser.hpp"

int main()
{
	Lexer lexer;
	LexerEnvironment* lexer_env = lexer.new_lexer_env("source.txt");
	lexer.tokenize(lexer_env);

	//lexer.print_tokens(lexer_env);

	Parser parser;
	ParserEnvironment* parser_env = parser.new_parser_env(lexer_env);
	parser.parse(parser_env);

	for (int i = 0; i < 256; i++)
	{
		ram.address[i] = parser_env->address[i];
	}

	run();
}