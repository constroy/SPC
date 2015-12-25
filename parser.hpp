/*
 * parser.hpp
 *
 * Copyright 2015 constroy <constroy.li@gmail.com>
 *
 *
 */


#ifndef PARSER_HPP
#define PARSER_HPP

#include "error.hpp"
#include "lexer.hpp"
#include "component.hpp"

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

#endif /* PARSER_HPP */
