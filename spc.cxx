/*
 * spc.cxx
 *
 * Copyright 2015 constroy <constroy.li@gmail.com>
 *
 *
 */

#include <cstdio>
#include "parser.hxx"

int main(int argc,char *argv[])
{
	if (argc!=3)
	{
		puts("usage: spc [src_name] [obj_name]");
		return 0;
	}
	//unit test [Paser]------------------------------------------------
	//freopen(argv[2],"w",stdout);
	Parser parser(argv[1]);
	Program *prog=parser.genAST();
	delete prog;
	//-----------------------------------------------------------------
	return 0;
}
