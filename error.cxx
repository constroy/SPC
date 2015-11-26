/*
 * error.cxx
 *
 *
 */


#include "error.hxx"

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

};

bool success=true;

void error(const Lexer &lexer,Error e)
{
	success=false;
	auto pos=lexer.getPos();
	fprintf(stderr,"r%dc%d: '%s': error: %s\n",
			pos.first,pos.second,lexer.currToken().s.c_str(),msg[e]);
}
void warning(const Lexer &lexer,Error w)
{
	auto pos=lexer.getPos();
	fprintf(stderr,"r%dc%d: '%s': warning: %s\n",
			pos.first,pos.second,lexer.currToken().s.c_str(),msg[w]);
}
