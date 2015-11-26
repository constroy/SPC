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
	~ConstDef();
	Token *name,*value;
};
struct VarDef
{
	VarDef(Lexer &lexer,bool f=true);
	~VarDef();
	vector<Token*> names;
	int size;
	Token *type;
};
struct ProcDef
{
	ProcDef(Lexer &lexer);
	~ProcDef();
	Token *name;
	vector<pair<bool,VarDef*>> para_list;
	Program *program;
};
struct FuncDef
{
	FuncDef(Lexer &lexer);
	~FuncDef();
	Token *name,*type;
	vector<pair<bool,VarDef*>> para_list;
	Program *program;
};
struct Expression;
struct ProcCall
{
	ProcCall(Token token,Lexer &lexer);
	~ProcCall();
	Token *name;
	vector<Expression*> arg_list;
};
struct Factor
{
	Factor(Lexer &lexer);
	~Factor();
	Token *token;
	Expression *exp;
	vector<Expression*> arg_list;
};
struct Term
{
	Term(Lexer &lexer);
	~Term();
	vector<pair<Token*,Factor*>> factors;
};
struct Expression
{
	Expression(Lexer &lexer);
	~Expression();
	vector<pair<Token*,Term*>> terms;
};
struct Condition
{
	Condition(Lexer &lexer);
	~Condition();
	Token *token;
	Expression *exp0,*exp1;
};
struct Block;
struct Assignment
{
	Assignment(Token token,Expression *exp,Lexer &lexer);
	~Assignment();
	Token *dest;
	Expression *exp0,*exp1;
};
struct Statement;
struct DoWhile
{
	DoWhile(Lexer &lexer);
	~DoWhile();
	Condition *condition;
	Statement *statement;
};
struct ForDo
{
	ForDo(Lexer &lexer);
	~ForDo();
	Token *token0,*token1;
	Expression *exp0,*exp1;
	Statement *statement;
};
struct IfThen
{
	IfThen(Lexer &lexer);
	~IfThen();
	Condition *condition;
	Statement *statement0,*statement1;
};
struct Read
{
	Read(Lexer &lexer);
	~Read();
	vector<Token*> tokens;
};
struct Write
{
	Write(Lexer &lexer);
	~Write();
	Token *token;
	Expression *exp;
};
struct Statement
{
	Statement(Lexer &lexer);
	~Statement();
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
	~Block();
	vector<Statement*> statements;
};
struct Program
{
	Program(Lexer &Lexer);
	~Program();
	vector<ConstDef*> const_defs;
	vector<VarDef*> var_defs;
	vector<ProcDef*> proc_defs;
	vector<FuncDef*> func_defs;
	Block *block;
};

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
		/* add your private declarations */
};

#endif /* PARSER_HXX */
