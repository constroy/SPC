/*
 * error.cxx
 *
 *
 */


#include "error.hxx"

char error_msg[][64]=
{
	"cannot open the source file",
	"unexpected end of file",
	"unknown charactor or symbol"
};
char warning_msg[][64]=
{
	"constant integer may overflow",
};

void error(int r,int c,Error e)
{
	fprintf(stderr,"r%dc%d: error: %s",r,c,error_msg[e]);
}

void warning(int r,int c,Warning w)
{
	fprintf(stderr,"r%dc%d: warning: %s",r,c,warning_msg[w]);
}
