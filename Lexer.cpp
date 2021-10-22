#include "Lexer.hpp"

std::string load_source(std::string filename)
{
    std::ifstream stream(filename);
    std::string source((std::istreambuf_iterator<char>(stream)),
        std::istreambuf_iterator<char>());
    return source;
}

LexerEnvironment* Lexer::new_lexer_env(std::string file_name)
{
    LexerEnvironment* lexer_env = new LexerEnvironment();
    lexer_env->source = load_source(file_name);
    lexer_env->source_length = lexer_env->source.length();
    lexer_env->index = 0;
    lexer_env->current_char = lexer_env->source[0];
    lexer_env->line = 1;
    lexer_env->column = 1;
    lexer_env->file_name = file_name;

    return lexer_env;
}

void Lexer::error_and_exit(LexerEnvironment* lexer_env, std::string message)
{
    std::string error_message = "\n\nLexical Error in '" + lexer_env->file_name + "' @ (" + std::to_string(lexer_env->line) + ", " + std::to_string(lexer_env->column) + "): " + message;
    std::cout << error_message;
    std::cout << "\nCompilation failed. Press any key to exit...";
    std::cin.get();
    exit(1);
}

void Lexer::advance(LexerEnvironment* lexer_env, int n = 1)
{
    if (lexer_env->current_char == '\0' || (lexer_env->index + n) > lexer_env->source_length)
    {
        lexer_env->index = lexer_env->source_length;
        lexer_env->current_char = lexer_env->source[lexer_env->index];
        return;
    }
    else
    {
        lexer_env->index = lexer_env->index + n;
        lexer_env->current_char = lexer_env->source[lexer_env->index];
        lexer_env->column++;
    }
}

char Lexer::peek(LexerEnvironment* lexer_env, int n = 1)
{
    if (lexer_env->current_char == '\0' || (lexer_env->index + n) >= lexer_env->source_length)
    {
        return '\0';
    }
    else
    {
        int ind = lexer_env->index + n;
        char c = lexer_env->source[ind];
        return c;
    }
}

void Lexer::build_word(LexerEnvironment* lexer_env)
{
    Token* token = new Token();
    token->type = TYPE_ID;
    token->line = lexer_env->line;
    token->column = lexer_env->column;

    std::string* name = new std::string();

    while (isalpha(lexer_env->current_char) || lexer_env->current_char == '_' || isdigit(lexer_env->current_char))
    {
        *name += lexer_env->current_char;
        advance(lexer_env);
    }

    token->id_name = name;
    token->literal_type = true;
    lexer_env->tokens.push_back(token);
}

void Lexer::build_number(LexerEnvironment* lexer_env)
{
    Token* token = new Token();
    token->line = lexer_env->line;
    token->column = lexer_env->column;

    std::string value;
    int num_dots = 0;

    if (lexer_env->current_char == '0' && peek(lexer_env) == 'x')
    {
        while (isdigit(lexer_env->current_char) || isalpha(lexer_env->current_char))
        {
            value += lexer_env->current_char;
            advance(lexer_env);
        }

        char* end = &lexer_env->current_char;

        token->hex_value = std::strtol(value.c_str(), &end, 16);
        token->type = TYPE_HEX;
        token->literal_type = true;
        lexer_env->tokens.push_back(token);
        return;
    }

    while (isdigit(lexer_env->current_char) || lexer_env->current_char == '.')
    {
        value += lexer_env->current_char;

        if (lexer_env->current_char == '.')
        {
            num_dots++;
        }

        advance(lexer_env);
    }

    if (num_dots == 0)
    {
        token->type = TYPE_INT;
        token->int_value = std::stoi(value);
    }
    else if (num_dots == 1)
    {
        token->type = TYPE_FLOAT;
        token->float_value = std::stof(value);
    }
    else
    {
        token->type = TYPE_ERROR;
    }

    token->literal_type = true;
    lexer_env->tokens.push_back(token);
}

void Lexer::build_string(LexerEnvironment* lexer_env)
{
    Token* token = new Token();
    token->type = TYPE_STRING;
    token->line = lexer_env-> line;
    token->column = lexer_env-> column;

    advance(lexer_env);
    lexer_env-> column++;

    std::string* value = new std::string();

    while (lexer_env-> current_char != '"')
    {
        if (lexer_env->current_char == '\0')
        {
            error_and_exit(lexer_env, "No closing quotation mark found, reached EOF.");
        }
        *value += lexer_env-> current_char;
        advance(lexer_env);
    }

    token->string_value = value;
    token->literal_type = true;
    lexer_env->tokens.push_back(token);

    advance(lexer_env);
}

void Lexer::build_char(LexerEnvironment* lexer_env)
{
    Token* token = new Token();
    token->type = TYPE_CHAR;
    token->line = lexer_env->line;
    token->column = lexer_env->column;

    advance(lexer_env);

    std::string value;

    while (lexer_env->current_char != '\'')
    {
        value += lexer_env->current_char;
        advance(lexer_env);
    }

    token->char_value = value[0];
    token->literal_type = true;
    lexer_env->tokens.push_back(token);

    advance(lexer_env);
}

void Lexer::add_token(LexerEnvironment* lexer_env, Type type, int size = 1, int op = false, int unary_op = false)
{
    Token* token = new Token();
    token->type = type;
    token->line = lexer_env->line;
    token->column = lexer_env->column;
    token->op = op;
    token->unary_op = unary_op;

    lexer_env->tokens.push_back(token);

    advance(lexer_env, size);
}

void Lexer::handle_line_comment(LexerEnvironment* lexer_env)
{
    advance(lexer_env, 2);

    while (lexer_env->current_char != '\n')
    {
        if (lexer_env->current_char == '\0')
        {
            return;
        }

        advance(lexer_env);
    }

    lexer_env->line++;
    lexer_env->column = 0;

    advance(lexer_env);

    return;
}

void Lexer::handle_block_comment(LexerEnvironment* lexer_env)
{
    advance(lexer_env, 2);

    while (!(lexer_env->current_char == '*' && peek(lexer_env) == '/'))
    {
        if (lexer_env->current_char == '\n')
        {
            lexer_env->line++;
            lexer_env->column = 0;
        }
        if (lexer_env->current_char == '\0')
        {
            return;
        }

        advance(lexer_env);
    }

    advance(lexer_env, 2);

    return;
}

void Lexer::tokenize(LexerEnvironment* lexer_env)
{
    while (lexer_env->current_char != '\0')
    {
        if (lexer_env->current_char == '\n')
        {
            lexer_env->line++;
            lexer_env->column = 0;
            advance(lexer_env);
        }
        else if (lexer_env->current_char == ' ' || lexer_env->current_char == '\t')
        {
            advance(lexer_env);
        }
        else if (isalpha(lexer_env->current_char) || lexer_env->current_char == '_')
        {
            build_word(lexer_env);
        }
        else if (isdigit(lexer_env->current_char))
        {
            build_number(lexer_env);
        }
        else if (lexer_env->current_char == '"')
        {
            build_string(lexer_env);
        }
        else if (lexer_env->current_char == '\'')
        {
            build_char(lexer_env);
        }
        else if (lexer_env->current_char == '/' && peek(lexer_env) == '/')
        {
            handle_line_comment(lexer_env);
        }
        else if (lexer_env->current_char == '/' && peek(lexer_env) == '*')
        {
            handle_block_comment(lexer_env);
        }
        else if (lexer_env->current_char == '=' && peek(lexer_env) == '=')
        {
            add_token(lexer_env, TYPE_EQ_CHECK, 2, true);
        }
        else if (lexer_env->current_char == '!' && peek(lexer_env) == '=')
        {
            add_token(lexer_env, TYPE_NOT_EQ_CHECK, 2, true, false);
        }
        else if (lexer_env->current_char == '<' && peek(lexer_env) == '=')
        {
            add_token(lexer_env, TYPE_LESS_OR_EQ, 2, true, false);
        }
        else if (lexer_env->current_char == '>' && peek(lexer_env) == '=')
        {
            add_token(lexer_env, TYPE_GREATER_OR_EQ, 2, true, false);
        }
        else if (lexer_env->current_char == '+' && peek(lexer_env) == '=')
        {
            add_token(lexer_env, TYPE_PLUS_EQ, 2, true, false);
        }
        else if (lexer_env->current_char == '-' && peek(lexer_env) == '=')
        {
            add_token(lexer_env, TYPE_MINUS_EQ, 2, true, false);
        }
        else if (lexer_env->current_char == '+' && peek(lexer_env) == '+')
        {
            add_token(lexer_env, TYPE_PLUS_PLUS, 2, true, false);
        }
        else if (lexer_env->current_char == '-' && peek(lexer_env) == '-')
        {
            add_token(lexer_env, TYPE_MINUS_MINUS, 2, true, false);
        }
        else if (lexer_env->current_char == '|' && peek(lexer_env) == '|')
        {
            add_token(lexer_env, TYPE_OR, 2, true, false);
        }
        else if (lexer_env->current_char == '&' && peek(lexer_env) == '&')
        {
            add_token(lexer_env, TYPE_AND, 2, true, false);
        }
        else if (lexer_env->current_char == ':' && peek(lexer_env) == ':')
        {
            add_token(lexer_env, TYPE_DOUBLE_COLON, 2, true, false);
        }
        else if (lexer_env->current_char == '=')
        {
            add_token(lexer_env, TYPE_EQUAL, 1, true, false);
        }
        else if (lexer_env->current_char == '+')
        {
            add_token(lexer_env, TYPE_PLUS, 1, true, false);
        }
        else if (lexer_env->current_char == '-')
        {
            add_token(lexer_env, TYPE_MINUS, 1, true, false);
        }
        else if (lexer_env->current_char == '*')
        {
            add_token(lexer_env, TYPE_MUL, 1, true, false);
        }
        else if (lexer_env->current_char == '/')
        {
            add_token(lexer_env, TYPE_DIV, 1, true, false);
        }
        else if (lexer_env->current_char == ';')
        {
            add_token(lexer_env, TYPE_SEMICOLON, 1);
        }
        else if (lexer_env->current_char == '<')
        {
            add_token(lexer_env, TYPE_LESS_THAN, 1, true, false);
        }
        else if (lexer_env->current_char == '>')
        {
            add_token(lexer_env, TYPE_GREATER_THAN, 1, true, false);
        }
        else if (lexer_env->current_char == '!')
        {
            add_token(lexer_env, TYPE_NOT, 1);
        }
        else if (lexer_env->current_char == '(')
        {
            add_token(lexer_env, TYPE_LPAREN, 1);
        }
        else if (lexer_env->current_char == ')')
        {
            add_token(lexer_env, TYPE_RPAREN, 1);
        }
        else if (lexer_env->current_char == '{')
        {
            add_token(lexer_env, TYPE_LBRACE, 1);
        }
        else if (lexer_env->current_char == '}')
        {
            add_token(lexer_env, TYPE_RBRACE, 1);
        }
        else if (lexer_env->current_char == '[')
        {
            add_token(lexer_env, TYPE_LBRACKET, 1);
        }
        else if (lexer_env->current_char == ']')
        {
            add_token(lexer_env, TYPE_RBRACKET, 1);
        }
        else if (lexer_env->current_char == '.')
        {
            add_token(lexer_env, TYPE_DOT, 1, true, false);
        }
        else if (lexer_env->current_char == ',')
        {
            add_token(lexer_env, TYPE_COMMA, 1, false, false);
        }
        else if (lexer_env->current_char == '#')
        {
            add_token(lexer_env, TYPE_HASH, 1, false, false);
        }
        else if (lexer_env->current_char == '^')
        {
            add_token(lexer_env, TYPE_CARET, 1, true, false);
        }
        else if (lexer_env->current_char == '%')
        {
            add_token(lexer_env, TYPE_MOD, 1, true, false);
        }
        else if (lexer_env->current_char == '$')
        {
            add_token(lexer_env, TYPE_DOLLAR, 1, true, false);
        }
        else if (lexer_env->current_char == ':')
        {
            add_token(lexer_env, TYPE_COLON, 1, true, false);
        }
        else
        {
            error_and_exit(lexer_env, "Unknown symbol '" + std::string(1, lexer_env->current_char) + "'.");
        }
    }

    add_token(lexer_env, TYPE_EOF);

}

void print_token(Token* token)
{
    
    if (token->type == TYPE_CHAR)
        std::cout << "Char('" << token->char_value << "')";
    else if (token->type == TYPE_INT)
        std::cout << "Int(" << token->int_value << ")";
    else if (token->type == TYPE_FLOAT)
        std::cout << "Float(" << token->float_value << ")";
    else if (token->type == TYPE_HEX)
        std::cout << "Hex(" << token->hex_value << ")";
    else if (token->type == TYPE_STRING)
        std::cout << "String(\"" << *token->string_value << "\")";
    else if (token->type == TYPE_ID)
        std::cout << "ID(" << *token->id_name << ")";
    else if (token->op || token->unary_op)
        std::cout << "Operator(" << type_to_string(token->type) << ")";
    else
        std::cout << "Token(" << type_to_string(token->type) << ")";

}

void Lexer::print_tokens(LexerEnvironment* lexer_env)
{
    std::cout << "\nFile: " << lexer_env->file_name;
    std::cout << "\nNumber of tokens: " << lexer_env->tokens.size();
    std::cout << "\n[ ";
    for (Token* token : lexer_env->tokens)
    {
        print_token(token);
        std::cout << " ";
    }
    std::cout << "]";
}
