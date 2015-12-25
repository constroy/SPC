/*
 * spc.cxx
 *
 * Copyright 2015 constroy <constroy.li@gmail.com>
 *
 *
 */

#include <cstdio>
#include "parser.hpp"
#include "analyser.hpp"

int main(int argc,char *argv[])
{
	if (argc!=3)
	{
		puts("usage: spc [src_name] [asm_name]");
		return 0;
	}
	//------------------------------------------------
	Parser parser(argv[1]);
	const Program *prog=parser.genAST();
	if (success)
	{
		Analyser analyser(prog,argv[2]);
		analyser.genCode();
	}
	parser.delAST();
	//------------------------------------------------
	return 0;
}
