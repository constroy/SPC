/*
 * spc.cxx
 *
 * Copyright 2015 constroy <constroy.li@gmail.com>
 *
 *
 */

#include <cstdio>
#include "lexer.hxx"
#include <set>

int main(int argc,char *argv[])
{
	if (argc!=3)
	{
		puts("usage: spc [src_name] [obj_name]");
		return 0;
	}
	Token token;
	Lexer lexer(argv[1]);
	//unit test [Lexer]------------------------------------------------
	const std::string type[]=
	{
		"ident",
		"number","character","string",
		"word_read","word_write",
		"word_int","word_char","word_array","word_of",
		"word_const","word_var","word_func","word_proc",
		"word_do","word_while","word_for","word_to","word_downto",
		"word_if","word_then","word_else","word_begin","word_end",
		"assign","plus","minus","times","slash","eql","neq","lss","leq","gtr","geq",
		"lbracket","rbracket","lparen","rparen","colon","comma","semicolon","period",

	};
	freopen(argv[2],"w",stdout);
	do
	{
		token=lexer.NextToken();
		printf("%-12s",type[token.type].c_str());
		if (token.type==number) printf("%d\n",token.v);
		else if (token.type==character)	printf("%c\n",token.v);
		else printf("%s\n",token.s.c_str());
	}
	while (token.type!=period);
	//-----------------------------------------------------------------
	return 0;
}
