/*
 * parser.cxx
 *
 * Copyright 2015 constroy <constroy.li@gmail.com>
 *
 *
 */


#include "parser.hxx"

Parser::Parser(char file[]): program(nullptr),lexer(file)
{
	lexer.nextToken();
}

Parser::~Parser()
{
	delAST();
}

const Program *Parser::genAST()
{
	program=new Program(lexer);
	if (lexer.currToken().type!=period)
		warning(lexer,lost_period);
	return program;
}

void Parser::delAST()
{
	delete program;
	program=nullptr;
}
