#include "Parser.hpp"

ParserEnvironment* Parser::new_parser_env(LexerEnvironment* lexer_env)
{
	ParserEnvironment* p_env = new ParserEnvironment();
	p_env->tokens = lexer_env->tokens;
	p_env->token = p_env->tokens[0];
	p_env->file_name = lexer_env->file_name;
	return p_env;
}

void Parser::parse(ParserEnvironment* p_env)
{
	while (p_env->token->type != TYPE_EOF)
	{
		if (p_env->token->type == TYPE_ID)
		{
			if (*p_env->token->id_name == "LDA")
			{
				advance(p_env);

				if (p_env->token->type == TYPE_INT)
				{
					p_env->address[p_env->current_address] = INST_LDA;
					p_env->current_address++;
					p_env->address[p_env->current_address] = p_env->token->int_value;
					p_env->current_address++;
				}
				else if (p_env->token->type == TYPE_DOLLAR)
				{
					advance(p_env);
					p_env->address[p_env->current_address] = INST_LDAI;
					p_env->current_address++;
					p_env->address[p_env->current_address] = p_env->token->int_value;
					p_env->current_address++;
				}
			}

			else if (*p_env->token->id_name == "LDB")
			{
				advance(p_env);

				if (p_env->token->type == TYPE_INT)
				{
					p_env->address[p_env->current_address] = INST_LDB;
					p_env->current_address++;
					p_env->address[p_env->current_address] = p_env->token->int_value;
					p_env->current_address++;
				}
				else if (p_env->token->type == TYPE_DOLLAR)
				{
					advance(p_env);
					p_env->address[p_env->current_address] = INST_LDBI;
					p_env->current_address++;
					p_env->address[p_env->current_address] = p_env->token->int_value;
					p_env->current_address++;
				}
			}
			else if (*p_env->token->id_name == "STA")
			{
				advance(p_env);

				if (p_env->token->type == TYPE_INT)
				{
					p_env->address[p_env->current_address] = INST_STA;
					p_env->current_address++;
					p_env->address[p_env->current_address] = p_env->token->int_value;
					p_env->current_address++;
				}
			}
			else if (*p_env->token->id_name == "JNZ")
			{
				advance(p_env);

				if (p_env->token->type == TYPE_INT)
				{
					p_env->address[p_env->current_address] = INST_JNZ;
					p_env->current_address++;
					p_env->address[p_env->current_address] = p_env->token->int_value;
					p_env->current_address++;
				}
				else if (p_env->token->type == TYPE_ID)
				{
					p_env->address[p_env->current_address] = INST_JNZ;
					p_env->current_address++;
					p_env->address[p_env->current_address] = p_env->labels[*p_env->token->id_name];
					p_env->current_address++;
				}
			}
			else if (*p_env->token->id_name == "JPZ")
			{
				advance(p_env);

				if (p_env->token->type == TYPE_INT)
				{
					p_env->address[p_env->current_address] = INST_JPZ;
					p_env->current_address++;
					p_env->address[p_env->current_address] = p_env->token->int_value;
					p_env->current_address++;
				}
				else if (p_env->token->type == TYPE_ID)
				{
					p_env->address[p_env->current_address] = INST_JPZ;
					p_env->current_address++;
					p_env->address[p_env->current_address] = p_env->labels[*p_env->token->id_name];
					p_env->current_address++;
				}
			}
			else if (*p_env->token->id_name == "JPZ")
			{
				advance(p_env);

				if (p_env->token->type == TYPE_INT)
				{
					p_env->address[p_env->current_address] = INST_JPZ;
					p_env->current_address++;
					p_env->address[p_env->current_address] = p_env->token->int_value;
					p_env->current_address++;
				}
			}
			else if (*p_env->token->id_name == "ADD")
			{
				p_env->address[p_env->current_address] = INST_ADD;
				p_env->current_address++;
			}
			else if (*p_env->token->id_name == "LDO")
			{
				p_env->address[p_env->current_address] = INST_LDO;
				p_env->current_address++;
			}
			else if (*p_env->token->id_name == "OUT")
			{
				p_env->address[p_env->current_address] = INST_OUT;
				p_env->current_address++;
			}
			else if (*p_env->token->id_name == "OUT_C")
			{
				p_env->address[p_env->current_address] = INST_OUT_C;
				p_env->current_address++;
			}
			else if (*p_env->token->id_name == "HLT")
			{
				p_env->address[p_env->current_address] = INST_HLT;
				p_env->current_address++;
			}
			else
			{
				// labels
				std::string label = *p_env->token->id_name;
				advance(p_env);
				if (p_env->token->type != TYPE_COLON)
					error_and_exit(p_env, "Expected ':'");
				else
				{
					p_env->labels[label] = p_env->current_address;
				}
			}
		}

		advance(p_env);
	}
}

void Parser::error_and_exit(ParserEnvironment* p_env, std::string message)
	{
		std::string error_message = "\nParsing Error in '" + p_env->file_name + "' @ (" + std::to_string(p_env->token->line) + ", " + std::to_string(p_env->token->column) + "): " + message;
		std::cout << error_message;
		std::cout << "\nCompilation failed. Press any key to exit...";
		std::cin.get();
		exit(1);
	}

void Parser::advance(ParserEnvironment* p_env)
{
	p_env->index++;
	p_env->token = p_env->tokens[p_env->index];
}