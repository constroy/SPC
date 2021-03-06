/*
 * error.hpp
 *
 * Copyright 2015 constroy <constroy.li@gmail.com>
 *
 *
 */


#ifndef ERROR_HPP
#define ERROR_HPP

#include <cstdio>
#include <utility>
#include "lexer.hpp"

enum Error
{
	//lexer
	open_failed,
	unexpected_EOF,
	unknown_character,
	integer_overflow,
	//parser
	lost_ident,
	lost_number,
	lost_num_or_char,
	lost_of,
	lost_type,
	lost_begin,
	lost_while,
	lost_assign,
	lost_to,
	lost_do,
	lost_then,
	lost_end,
	lost_period,
	lost_colon,
	lost_semicolon,
	lost_lparen,
	lost_rparen,
	lost_lbracket,
	lost_rbracket,
	lost_eql,
	lost_relational,
	empty_statement,
	//analyser
	not_array,
	lost_subscript,
	not_value,
	cannot_call,
	cannot_read,
	wrong_arg_num,
	ref_rvalue,
	//symtab
	redeclaration,
	undefinition,
};

extern const char msg[][64];
extern bool success;

void error(const Lexer &lexer,Error e);
void error(const std::string &s,Error e);
void warning(const Lexer &lexer,Error w);

#endif /* ERROR_HPP */
