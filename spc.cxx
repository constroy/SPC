/*
 * spc.cxx
 *
 * Copyright 2015 constroy <constroy.li@gmail.com>
 *
 *
 */

#include <cstdio>
#include "lexer.hxx"

int main(int argc,char *argv[])
{
	if (argc!=3)
	{
		puts("usage: spc [src_name] [obj_name]");
		exit(-1);
	}
	Token token;
	Lexer lexer(fopen(argv[1],"r"));
	//code for test-----------------------------------------
	freopen(argv[2],"w",stdout);
	do
	{
		token=lexer.NextToken();
		if (token.type==ident)
		{
			printf("ident\t%s\n",token.s.c_str());
		}
		else if (token.type==number)
		{
			printf("number\t%d\n",token.v);
		}
		else if (token.type==character)
		{
			printf("character\t%c\n",token.v);
		}
		else if (token.type==string)
		{
			printf("string\t%s\n",token.s.c_str());
		}
		else
		{
			for (auto i:word) if (token.type==i.second)
				printf("%s\t%s\n",i.first.c_str(),token.s.c_str());
			for (auto i:symb) if (token.type==i.second)
				printf("%s\t%s\n",i.first.c_str(),token.s.c_str());
		}
	}
	while (token.type!=period);
	//------------------------------------------------------
	return 0;
}
