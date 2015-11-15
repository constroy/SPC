/*
 * lexer.cxx
 *
 * Copyright 2015 constroy <constroy.li@gmail.com>
 *
 *
 */


#include "lexer.hxx"

const map<std::string,TokenType> Lexer::word=
{
	{"read",readsym},{"write",writesym},
	{"integer",intsym},{"char",charsym},{"array",arraysym},
	{"const",constsym},{"var",varsym},
	{"function",funcsym},{"procedure",procsym},
	{"begin",beginsym},{"end",endsym},{"do",dosym},{"while",whilesym},
	{"for",forsym},{"to",tosym},{"downto",downtosym},
	{"if",ifsym},{"then",thensym},{"else",elsesym}
};

const map<std::string,TokenType> Lexer::symb=
{
	{":=",become},{"+",plus},{"-",minus},{"*",times},{"/",slash},
	{"=",eql},{"<>",neq},{"<",lss},{"<=",leq},{">",gtr},{">=",geq},
	{"[",lbracket},{"]",rbracket},{"(",lparen},{")",rparen},
	{":",colon},{",",comma},{";",semicolon},{".",period}
};

Lexer::Lexer(char file[])
{
	src=fopen(file,"r");
	if (!src) exit(-3);
	chr=fgetc(src);
}
Lexer::~Lexer()
{
	fclose(src);
}
Token Lexer::NextToken()
{
	while (isspace(chr)) chr=fgetc(src);
	if (chr==EOF)
	{
		exit(-1);
	}
	else if (isalpha(chr))
	{
		std::string buff;
		while (isalnum(chr))
		{
			buff+=chr;
			chr=fgetc(src);
		}
		if (word.count(buff))
		{
			token.type=word.at(buff);
			token.s=buff;
		}
		else
		{
			token.type=ident;
			token.s=buff;
		}
	}
	else if (isdigit(chr))
	{
		int num=0;
		while (isdigit(chr))
		{
			num=num*10+chr-'0';
			chr=fgetc(src);
		}
		token.type=number;
		token.v=num;
	}
	else if (ispunct(chr))
	{
		std::string buff;
		if (chr=='\'')
		{
			chr=fgetc(src);
			if (isalnum(chr) && fgetc(src)=='\'')
			{
				token.type=character;
				token.v=chr;
			}
			else
			{
				exit(-2);
			}
		}
		else if (chr=='"')
		{
			chr=fgetc(src);
			while (isprint(chr) && chr!='"')
			{
				buff+=chr;
				chr=fgetc(src);
			}
			if (chr=='"')
			{
				chr=fgetc(src);
				token.type=string;
				token.s=buff;
			}
			else
			{
				exit(-2);
			}
		}
		else
		{
			buff+=chr;
			chr=fgetc(src);
			if (chr=='=' || chr=='>')
			{
				buff+=chr;
				chr=fgetc(src);
			}
			if (symb.count(buff))
			{
				token.type=symb.at(buff);
				token.s=buff;
			}
			else
			{
				exit(-2);
			}
		}
	}
	else
	{
		exit(-2);
	}
	return token;
}
