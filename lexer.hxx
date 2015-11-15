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
	readsym,writesym,
	intsym,charsym,arraysym,
	constsym,varsym,funcsym,procsym,
	become,plus,minus,times,slash,eql,neq,lss,leq,gtr,geq,
	lbracket,rbracket,lparen,rparen,colon,comma,semicolon,period,
	beginsym,endsym,dosym,whilesym,forsym,tosym,downtosym,ifsym,thensym,elsesym
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
		Token token;
};

#endif /* LEXER_HXX */