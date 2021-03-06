/*
 * lexer.hpp
 *
 * Copyright 2015 constroy <constroy.li@gmail.com>
 *
 *
 */


#ifndef LEXER_HPP
#define LEXER_HPP

#include <cstdlib>
#include <cstdio>
#include <climits>
#include <string>
#include <map>

typedef std::pair<int,int> Pos;

enum TokenType
{
	none,
	ident,
	number,character,string,
	word_read,word_write,
	word_integer,word_char,word_array,word_of,
	word_const,word_var,word_proc,word_func,
	word_do,word_while,word_for,word_to,word_downto,
	word_if,word_then,word_else,word_begin,word_end,
	assign,plus,minus,times,slash,eql,neq,lss,geq,leq,gtr,
	lbracket,rbracket,lparen,rparen,colon,comma,semicolon,period
};

struct Token
{
	TokenType type;
	std::string s;
	int v;
	bool operator ==(const Token &other)
	{
		return type==other.type && s==other.s && v==other.v;
	}
};

class Lexer
{
	public:
		Lexer(const char file[]);
		~Lexer();
		const Pos &getPos() const;
		const Token &currToken() const;
		const Token &nextToken();
	private:
		static const std::map<std::string,TokenType> word;
		static const std::map<std::string,TokenType> symb;
		FILE *const src;
		int chr;
		Pos pos;
		int row,col;
		Token token;
		void read();
};

#endif /* LEXER_HPP */
