#include "Type.hpp"

std::string type_to_string(int type)
{
	switch (type)
	{
	case TYPE_EMPTY:
		return "empty";
	case TYPE_CHAR:
		return "character";
	case TYPE_INT:
		return "integer";
	case TYPE_FLOAT:
		return "float";
	case TYPE_HEX:
		return "hex";
	case TYPE_STRING:
		return "string";
	case TYPE_ID:
		return "identifier";
	case TYPE_PLUS:
		return "+";
	case TYPE_MINUS:
		return "-";
	case TYPE_MUL:
		return "*";
	case TYPE_DIV:
		return "/";
	case TYPE_POS:
		return "+";
	case TYPE_NEG:
		return "-";
	case TYPE_EQUAL:
		return "=";
	case TYPE_LPAREN:
		return "(";
	case TYPE_RPAREN:
		return ")";
	case TYPE_LBRACE:
		return "{";
	case TYPE_RBRACE:
		return "}";
	case TYPE_LBRACKET:
		return "[";
	case TYPE_RBRACKET:
		return "]";
	case TYPE_SEMICOLON:
		return ";";
	case TYPE_EQ_CHECK:
		return "==";
	case TYPE_NOT_EQ_CHECK:
		return "!=";
	case TYPE_LESS_OR_EQ:
		return "<=";
	case TYPE_GREATER_OR_EQ:
		return ">=";
	case TYPE_LESS_THAN:
		return "<";
	case TYPE_GREATER_THAN:
		return ">";
	case TYPE_HASH:
		return "#";
	case TYPE_CARET:
		return "^";
	case TYPE_MOD:
		return "%";
	case TYPE_COLON:
		return ":";
	case TYPE_DOLLAR:
		return "$";
	case TYPE_FUNC_CALL:
		return "function call";
	case TYPE_EOF:
		return "EOF";
	default:
		return "repr_undefined";
	}
}