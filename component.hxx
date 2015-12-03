/*
 * parser.hxx
 *
 * Copyright 2015 constroy <constroy.li@gmail.com>
 *
 *
 */


#ifndef COMPONENT_HXX
#define COMPONENT_HXX

#include <utility>
#include <vector>
#include "lexer.hxx"
#include "coder.hxx"
#include "optimizer.hxx"
#include "symtab.hxx"

using std::pair;
using std::make_pair;
using std::vector;

struct Program;

struct ConstDef
{
	ConstDef(Lexer &lexer);
	~ConstDef();
	void genCode(Coder &coder,SymTab &symtab) const;
	Token *name,*value;
};
struct VarDef
{
	VarDef(Lexer &lexer,bool f=true);
	~VarDef();
	void genCode(Coder &coder,SymTab &symtab) const;
	vector<Token*> names;
	int size;
	Token *type;
};
struct ProcDef
{
	ProcDef(Lexer &lexer);
	~ProcDef();
	void genCode(Coder &coder,SymTab &symtab) const;
	Token *name;
	vector<pair<bool,VarDef*>> para_list;
	Program *program;
};
struct FuncDef
{
	FuncDef(Lexer &lexer);
	~FuncDef();
	void genCode(Coder &coder,SymTab &symtab) const;
	Token *name,*type;
	vector<pair<bool,VarDef*>> para_list;
	Program *program;
};

struct Expression;

struct Factor
{
	Factor(Lexer &lexer);
	~Factor();
	void genCode(Coder &coder,SymTab &symtab) const;
	Token *token;
	Expression *exp;
	vector<Expression*> arg_list;
};

struct Term
{
	Term(Lexer &lexer);
	~Term();
	void genCode(Coder &coder,SymTab &symtab) const;
	vector<pair<Token*,Factor*>> factors;
};

struct Expression
{
	Expression(Lexer &lexer);
	~Expression();
	void genCode(Coder &coder,SymTab &symtab) const;
	vector<pair<Token*,Term*>> terms;
};

struct Condition
{
	Condition(Lexer &lexer);
	~Condition();
	void genCode(Coder &coder,SymTab &symtab) const;
	Token *token;
	Expression *exp0,*exp1;
};

struct Statement
{
	static Statement *bear(Lexer &lexer);
	virtual ~Statement()=0;
	virtual void genCode(Coder &coder,SymTab &symtab) const=0;
};


struct Assignment: Statement
{
	Assignment(Token token,Expression *exp,Lexer &lexer);
	~Assignment() override;
	virtual void genCode(Coder &coder,SymTab &symtab) const override;
	Token *dest;
	Expression *exp0,*exp1;
};

struct ProcCall: Statement
{
	ProcCall(Token token,Lexer &lexer);
	~ProcCall();
	virtual void genCode(Coder &coder,SymTab &symtab) const override;
	Token *name;
	vector<Expression*> arg_list;
};

struct DoWhile: Statement
{
	DoWhile(Lexer &lexer);
	~DoWhile() override;
	virtual void genCode(Coder &coder,SymTab &symtab) const override;
	Condition *condition;
	Statement *statement;
};
struct ForDo: Statement
{
	ForDo(Lexer &lexer);
	~ForDo() override;
	virtual void genCode(Coder &coder,SymTab &symtab) const override;
	Token *token0,*token1;
	Expression *exp0,*exp1;
	Statement *statement;
};
struct IfThen: Statement
{
	IfThen(Lexer &lexer);
	~IfThen() override;
	virtual void genCode(Coder &coder,SymTab &symtab) const override;
	Condition *condition;
	Statement *statement0,*statement1;
};
struct Read: Statement
{
	Read(Lexer &lexer);
	~Read() override;
	virtual void genCode(Coder &coder,SymTab &symtab) const override;
	vector<Token*> tokens;
};
struct Write: Statement
{
	Write(Lexer &lexer);
	~Write() override;
	virtual void genCode(Coder &coder,SymTab &symtab) const override;
	Token *token;
	Expression *exp;
};

struct Block: Statement
{
	Block(Lexer &lexer);
	~Block();
	virtual void genCode(Coder &coder,SymTab &symtab) const override;
	vector<Statement*> statements;
};
struct Program
{
	Program(Lexer &Lexer);
	~Program();
	void genCode(Coder &coder,SymTab &symtab) const;
	vector<ConstDef*> const_defs;
	vector<VarDef*> var_defs;
	vector<ProcDef*> proc_defs;
	vector<FuncDef*> func_defs;
	Block *block;
};

#endif /* COMPONENT_HXX */
