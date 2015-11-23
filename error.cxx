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

void error(Pos pos,Error e)
{
	fprintf(stderr,"r%dc%d: error: %s",pos.first,pos.second,msg[e]);
}
void warning(Pos pos,Error w)
{
	fprintf(stderr,"r%dc%d: warning: %s",pos.first,pos.second,msg[w]);
}
