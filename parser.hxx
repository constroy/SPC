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
using std::vector;

struct Program;

struct ConstDef
{
	Token *name;
	Token *value;
};
struct VarDef
{
	vector<Token*> names;
	Token *type;
	bool array;
};
struct ConstDecl
{
	vector<ConstDef*> const_defs;
};
struct VarDecl
{
	vector<VarDef*> var_defs;
};
struct FuncDef
{
	Token *name;
	Token *type;
	vector<pair<bool,VarDef>*> para_list;
	Program *program;
};
struct ProcDef
{
	Token *name;
	vector<pair<bool,VarDef>*> para_list;
	VarDecl *ref_para_decl;
	Program *program;
};
struct Expression;

struct FuncCall
{
	Token *name;
	vector<Expression*> para_list;
};
struct ProcCall
{
	Token *name;
	VarDecl *val_para_decl;
	VarDecl *ref_para_decl;
};
struct Factor
{
	Token *token;
	Expression *exp;
	FuncCall *func_call;
};
struct Term
{
	vector<pair<Token*,Factor*>> factors;
};
struct Expression
{
	vector<pair<Token*,Term*>> terms;
};
struct Condition
{
	Token token;
	Expression exp0,exp1;
};
struct Block;
struct Assignment
{
	Token *token;
	Expression *exp;
};
struct Statement;
struct DoWhile
{
	Condition *condition;
	Statement *statement;
};
struct ForDo
{
	Token token;
	Expression *exp0,*exp1;
	bool down;
	Statement *statement;
};
struct IfThen
{
	Condition *condition;
	Statement *statement0,*statement1;
};
struct Write
{
	Token *token;
	Expression *exp;
};
struct Read
{
	vector<Token*> tokens;
};
struct Statement
{
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
	vector<Statement*> statements;
};
struct Program
{
	ConstDecl *const_decl;
	VarDecl *var_decl;
	vector<FuncDef*> func_defs;
	vector<ProcDef*> proc_defs;
	Block *block;
};

class Parser
{
	public:
		Parser();

	private:
		/* add your private declarations */
};

#endif /* PARSER_HXX */
