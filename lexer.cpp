/*
 * lexer.cxx
 *
 * Copyright 2015 constroy <constroy.li@gmail.com>
 *
 *
 */


#include "lexer.hpp"
#include "error.hpp"

const std::map<std::string,TokenType> Lexer::word=
{
	{"read",word_read},{"write",word_write},
	{"const",word_const},{"var",word_var},
	{"integer",word_integer},{"char",word_char},
	{"array",word_array},{"of",word_of},
	{"procedure",word_proc},{"function",word_func},
	{"begin",word_begin},{"end",word_end},
	{"do",word_do},{"while",word_while},
	{"for",word_for},{"to",word_to},{"downto",word_downto},
	{"if",word_if},{"then",word_then},{"else",word_else}
};
const std::map<std::string,TokenType> Lexer::symb=
{
	{":=",assign},{"+",plus},{"-",minus},{"*",times},{"/",slash},
	{"=",eql},{"<>",neq},{"<",lss},{"<=",leq},{">",gtr},{">=",geq},
	{"[",lbracket},{"]",rbracket},{"(",lparen},{")",rparen},
	{":",colon},{",",comma},{";",semicolon},{".",period}
};

Lexer::Lexer(const char file[]): src(fopen(file,"r"))
{
	if (!src)
	{
		error(std::string(file),open_failed);
		exit(1);
	}
	row=1,col=0;
	read();
}

Lexer::~Lexer()
{
	fclose(src);
}

const Pos &Lexer::getPos() const
{
	return pos;
}

const Token &Lexer::currToken() const
{
	return token;
}

const Token &Lexer::nextToken()
{
	while (isspace(chr)) read();
	if (isalpha(chr))
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
			if (num>USHRT_MAX) warning(*this,integer_overflow);
			read();
		}
		token.type=number;
		token.s=std::to_string(num);
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
				error(*this,unknown_character);
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
				error(*this,unknown_character);
			}
		}
		else
		{
			buff+=chr;
			read();
			if ((chr=='=' || chr=='>') && symb.count(buff+(char)chr))
			{
				buff+=chr;
				token.type=symb.at(buff);
				token.s=buff;
				read();
			}
			else if (symb.count(buff))
			{
				token.type=symb.at(buff);
				token.s=buff;
			}
			else
			{
				error(*this,unknown_character);
			}
		}
	}
	else
	{
		error(*this,unknown_character);
	}
	return token;
}

void Lexer::read()
{
	chr=fgetc(src);
	pos=std::make_pair(row,col);
	if (chr=='\n') ++row,col=1;
	else ++col;
	if (chr==EOF)
	{
		error(*this,unexpected_EOF);
		exit(1);
	}
}
