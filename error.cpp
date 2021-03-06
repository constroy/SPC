/*
 * error.cxx
 *
 *
 */


#include "error.hpp"

const char msg[][64]=
{
	//lexer
	"cannot open the source file",
	"unexpected end of file",
	"unknown charactor or symbol",
	"constant integer may overflow",
	//parser
	"identifier expected",
	"number expected",
	"number or character expected",
	"'of' expected",
	"type expected",
	"'begin' expected",
	"'while' expected",
	"':=' expected",
	"'to' or 'downto' expected",
	"'do' expected",
	"'then' expected",
	"'end' expected",
	"'.' expected",
	"':' expected",
	"';' expected",
	"'(' expected",
	"')' expected",
	"'[' expected",
	"']' expected",
	"'=' expected",
	"relational operator expected",
	"empty statement",
	//analyser
	"is not an array",
	"subscript expected",
	"is not a value",
	"cannot call",
	"cannot read",
	"too few/many arguments",
	"rvalue reference",
	//symtab
	"symbol redeclared",
	"symbol undefined"
};

bool success=true;

void error(const Lexer &lexer,Error e)
{
	success=false;
	auto pos=lexer.getPos();
	fprintf(stderr,"r%dc%d: '%s': error: %s\n",
			pos.first,pos.second,lexer.currToken().s.c_str(),msg[e]);
}

void error(const std::string &s,Error e)
{
	success=false;
	fprintf(stderr,"error: %s: %s\n",s.c_str(),msg[e]);
}

void warning(const Lexer &lexer,Error w)
{
	auto pos=lexer.getPos();
	fprintf(stderr,"r%dc%d: '%s': warning: %s\n",
			pos.first,pos.second,lexer.currToken().s.c_str(),msg[w]);
}
