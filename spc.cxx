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
		exit(-1);
	}
	Token token;
	Lexer lexer(argv[1]);
	//unit test [Lexer]------------------------------------------------
	const std::string type[]=
	{
		"ident",
		"number","character","string",
		"readsym","writesym",
		"intsym","charsym","arraysym",
		"constsym","varsym","funcsym","procsym",
		"become","plus","minus","times","slash","eql","neq","lss","leq","gtr","geq",
		"lbracket","rbracket","lparen","rparen","colon","comma","semicolon","period",
		"beginsym","endsym","dosym","whilesym","forsym","tosym","downtosym","ifsym","thensym","elsesym"
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
