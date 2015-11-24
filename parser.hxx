/*
 * parser.hxx
 *
 * Copyright 2015 constroy <constroy.li@gmail.com>
 *
 *
 */


#ifndef PARSER_HXX
#define PARSER_HXX

#include <utility>
#include <vector>
#include "lexer.hxx"

using std::pair;
using std::make_pair;
using std::vector;

struct Program;
struct ConstDef
{
	ConstDef(Lexer &lexer);
	Token *name,*value;
};
struct VarDef
{
	VarDef(Lexer &lexer,bool f=true);
	vector<Token*> names;
	int size;
	Token *type;
};
struct ProcDef
{
	ProcDef(Lexer &lexer);
	Token *name;
	vector<pair<bool,VarDef*>> para_list;
	Program *program;
};
struct FuncDef
{
	FuncDef(Lexer &lexer);
	Token *name,*type;
	vector<pair<bool,VarDef*>> para_list;
	Program *program;
};
struct Expression;
struct ProcCall
{
	ProcCall(Token token,Lexer &lexer);
	Token *name;
	vector<Expression*> para_list;
};
struct Factor
{
	Factor(Lexer &lexer);
	Token *token;
	Expression *exp;
	vector<Expression*> para_list;
};
struct Term
{
	Term(Lexer &lexer);
	vector<pair<Token*,Factor*>> factors;
};
struct Expression
{
	Expression(Lexer &lexer);
	vector<pair<Token*,Term*>> terms;
};
struct Condition
{
	Condition(Lexer &lexer);
	Token *token;
	Expression *exp0,*exp1;
};
struct Block;
struct Assignment
{
	Assignment(Token token,Expression *exp,Lexer &lexer);
	Token *dest;
	Expression *exp0,*exp1;
};
struct Statement;
struct DoWhile
{
	DoWhile(Lexer &lexer);
	Condition *condition;
	Statement *statement;
};
struct ForDo
{
	ForDo(Lexer &lexer);
	Token token0,token1;
	Expression *exp0,*exp1;
	Statement *statement;
};
struct IfThen
{
	IfThen(Lexer &lexer);
	Condition *condition;
	Statement *statement0,*statement1;
};
struct Read
{
	Read(Lexer &lexer);
	vector<Token*> tokens;
};
struct Write
{
	Write(Lexer &lexer);
	Token *token;
	Expression *exp;
};
struct Statement
{
	Statement(Lexer &lexer);
	Assignment *assignment;
	ProcCall *proc_call;
	DoWhile *do_while;
	ForDo *for_do;
	IfThen *if_then;
	Read *read;
	Write *write;
	Block *block;
};
struct Block
{
	Block(Lexer &lexer);
	vector<Statement*> statements;
};
struct Program
{
	Program(Lexer &Lexer);
	vector<ConstDef*> const_defs;
	vector<VarDef*> var_defs;
	vector<FuncDef*> func_defs;
	vector<ProcDef*> proc_defs;
	Block *block;
};

class Parser
{
	public:
		Parser(char file[]);
		Program *genAST();
	private:
		Program *program;
		Lexer lexer;
		Token token;
		/* add your private declarations */
};

#endif /* PARSER_HXX */
