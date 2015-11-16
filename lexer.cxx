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
	{"read",word_read},{"write",word_write},
	{"const",word_const},{"var",word_var},
	{"integer",word_int},{"char",word_char},
	{"array",word_array},{"of",word_of},
	{"function",word_func},{"procedure",word_proc},
	{"begin",word_begin},{"end",word_end},
	{"do",word_do},{"while",word_while},
	{"for",word_for},{"to",word_to},{"downto",word_downto},
	{"if",word_if},{"then",word_then},{"else",word_else}
};

const map<std::string,TokenType> Lexer::symb=
{
	{":=",assign},{"+",plus},{"-",minus},{"*",times},{"/",slash},
	{"=",eql},{"<>",neq},{"<",lss},{"<=",leq},{">",gtr},{">=",geq},
	{"[",lbracket},{"]",rbracket},{"(",lparen},{")",rparen},
	{":",colon},{",",comma},{";",semicolon},{".",period}
};

Lexer::Lexer(char file[])
{
	src=fopen(file,"r");
	if (!src) exit(-3);//Error
	row=1,col=0;
	read();
}
Lexer::~Lexer()
{
	fclose(src);
}
Token Lexer::NextToken()
{
	while (isspace(chr)) read();
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
			read();
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
			read();
		}
		token.type=number;
		token.v=num;
	}
	else if (ispunct(chr))
	{
		std::string buff;
		if (chr=='\'')
		{
			read();
			if (isalnum(chr) && fgetc(src)=='\'')
			{
				token.type=character;
				token.v=chr;
				read();
			}
			else
			{
				exit(-2);
			}
		}
		else if (chr=='"')
		{
			read();
			while (isprint(chr) && chr!='"')
			{
				buff+=chr;
				read();
			}
			if (chr=='"')
			{
				token.type=string;
				token.s=buff;
				read();
			}
			else
			{
				exit(-2);
			}
		}
		else
		{
			buff+=chr;
			read();
			if (chr=='=' || chr=='>')
			{
				buff+=chr;
				read();
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

void Lexer::read()
{
	chr=fgetc(src);
	if (chr=='\n') ++row,col=0;
	else ++col;
	if (chr==EOF) ;//TODO:Error;
}
