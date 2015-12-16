/*
 * parser.hxx
 *
 * Copyright 2015 constroy <constroy.li@gmail.com>
 *
 *
 */


#ifndef PARSER_HXX
#define PARSER_HXX

#include "error.hxx"
#include "lexer.hxx"
#include "component.hxx"

class Parser
{
	public:
		Parser(char file[]);
		~Parser();
		const Program *genAST();
		void delAST();
	private:
		Program *program;
		Lexer lexer;
		Token token;
};

#endif /* PARSER_HXX */
