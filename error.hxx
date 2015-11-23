/*
 * error.hxx
 *
 * Copyright 2015 constroy <constroy.li@gmail.com>
 *
 *
 */


#ifndef ERROR_HXX
#define ERROR_HXX

#include <cstdio>
#include <utility>

typedef std::pair<int,int> Pos;

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
	lost_lbracket,
	lost_rbracket,
	lost_lparen,
	lost_rparen,
	lost_eql,
	lost_relational,

};

extern const char msg[][64];

void error(Pos pos,Error e);
void warning(Pos pos,Error w);

#endif /* ERROR_HXX */
