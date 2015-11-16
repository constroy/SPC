/*
 * lexer.hxx
 *
 * Copyright 2015 constroy <constroy.li@gmail.com>
 *
 *
 */


#ifndef LEXER_HXX
#define LEXER_HXX

#include <cstdio>
#include <string>
#include <map>

using std::map;

enum TokenType
{
	ident,
	number,character,string,
	word_read,word_write,
	word_int,word_char,word_array,word_of,
	word_const,word_var,word_func,word_proc,
	word_do,word_while,word_for,word_to,word_downto,
	word_if,word_then,word_else,word_begin,word_end,
	assign,plus,minus,times,slash,eql,neq,lss,leq,gtr,geq,
	lbracket,rbracket,lparen,rparen,colon,comma,semicolon,period
};

struct Token
{
	TokenType type;
	int v;
	std::string s;
};

class Lexer
{
	public:
		Lexer(char file[]);
		~Lexer();
		Token NextToken();
	private:
		static const map<std::string,TokenType> word;
		static const map<std::string,TokenType> symb;
		FILE *src;
		int chr;
		int row,col;
		Token token;
		void read();

};

#endif /* LEXER_HXX */
