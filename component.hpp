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
using std::to_string;

struct Program;

struct ConstDef
{
	ConstDef(Lexer &lexer);
	~ConstDef();
	void scan(Coder &coder,SymTab &symtab) const;
	Token *name,*value;
};

struct VarDef
{
	VarDef(Lexer &lexer);
	~VarDef();
	void scan(Coder &coder,SymTab &symtab) const;
	vector<Token*> names;
	int size;
	Token *type;
};

struct ParaDef
{
	ParaDef(Lexer &lexer);
	~ParaDef();
	void scan(Coder &coder,SymTab &symtab) const;
	vector<Token*> names;
	int size;
	Token *type;
};

struct ProcDef
{
	ProcDef(Lexer &lexer);
	~ProcDef();
	void scan(Coder &coder,SymTab &symtab) const;
	void genCode(Coder &coder,SymTab &symtab) const;
	Token *name;
	vector<pair<bool,ParaDef*>> para_list;
	Program *program;
};

struct FuncDef
{
	FuncDef(Lexer &lexer);
	~FuncDef();
	void scan(Coder &coder,SymTab &symtab) const;
	void genCode(Coder &coder,SymTab &symtab) const;
	Token *name,*type;
	vector<pair<bool,ParaDef*>> para_list;
	Program *program;
};

struct Expression;

struct Factor
{
	Factor(Lexer &lexer);
	~Factor();
	void scan(Coder &coder,SymTab &symtab);
	void genCode(Coder &coder,SymTab &symtab) const;
	Token *token;
	Expression *exp;
	vector<Expression*> arg_list;
	int i;
	Token res;
};

struct Term
{
	Term(Lexer &lexer);
	~Term();
	bool isVar(const SymTab &symtab) const;
	Symbol getVar(const SymTab &symtab) const;
	void scan(Coder &coder,SymTab &symtab);
	void genCode(Coder &coder,SymTab &symtab) const;
	vector<pair<Token*,Factor*>> factors;
	Token res;
};

struct Expression
{
	Expression(Lexer &lexer);
	~Expression();
	bool isVar(const SymTab &symtab) const;
	Symbol getVar(const SymTab &symtab) const;
	void scan(Coder &coder,SymTab &symtab);
	void genCode(Coder &coder,SymTab &symtab) const;
	vector<pair<Token*,Term*>> terms;
	Token res;
};

struct Condition
{
	Condition(Lexer &lexer);
	~Condition();
	void scan(Coder &coder,SymTab &symtab);
	void genCode(Coder &coder,SymTab &symtab,bool res) const;
	Token *token;
	Expression *exp0,*exp1;
};

struct Statement
{
	static Statement *bear(Lexer &lexer);
	virtual ~Statement()=0;
	virtual void scan(Coder &coder,SymTab &symtab)=0;
	virtual void genCode(Coder &coder,SymTab &symtab) const=0;
};

struct Assignment: Statement
{
	Assignment(Token token,Expression *exp,Lexer &lexer);
	~Assignment() override;
	virtual void scan(Coder &coder,SymTab &symtab) override;
	virtual void genCode(Coder &coder,SymTab &symtab) const override;
	Token *dest;
	Expression *exp0,*exp1;
};

struct ProcCall: Statement
{
	ProcCall(Token token,Lexer &lexer);
	~ProcCall() override;
	virtual void scan(Coder &coder,SymTab &symtab) override;
	virtual void genCode(Coder &coder,SymTab &symtab) const override;
	Token *name;
	vector<Expression*> arg_list;
};

struct DoWhile: Statement
{
	DoWhile(Lexer &lexer);
	~DoWhile() override;
	virtual void scan(Coder &coder,SymTab &symtab) override;
	virtual void genCode(Coder &coder,SymTab &symtab) const override;
	Statement *statement;
	Condition *condition;
};

struct ForDo: Statement
{
	ForDo(Lexer &lexer);
	~ForDo() override;
	virtual void scan(Coder &coder,SymTab &symtab) override;
	virtual void genCode(Coder &coder,SymTab &symtab) const override;
	Token *token0,*token1;
	Expression *exp0,*exp1;
	Statement *statement;
};

struct IfThen: Statement
{
	IfThen(Lexer &lexer);
	~IfThen() override;
	virtual void scan(Coder &coder,SymTab &symtab) override;
	virtual void genCode(Coder &coder,SymTab &symtab) const override;
	Condition *condition;
	Statement *statement0,*statement1;
};
struct Read: Statement
{
	Read(Lexer &lexer);
	~Read() override;
	virtual void scan(Coder &coder,SymTab &symtab) override;
	virtual void genCode(Coder &coder,SymTab &symtab) const override;
	vector<Token*> tokens;
};
struct Write: Statement
{
	Write(Lexer &lexer);
	~Write() override;
	virtual void scan(Coder &coder,SymTab &symtab) override;
	virtual void genCode(Coder &coder,SymTab &symtab) const override;
	Token *token;
	Expression *exp;
};

struct Block: Statement
{
	Block(Lexer &lexer);
	~Block();
	virtual void scan(Coder &coder,SymTab &symtab) override;
	virtual void genCode(Coder &coder,SymTab &symtab) const override;
	vector<Statement*> statements;
};
struct Program
{
	Program(Lexer &Lexer);
	~Program();
	void scan(Coder &coder,SymTab &symtab) const;
	void genCode(Coder &coder,SymTab &symtab,const std::string &val) const;
	vector<ConstDef*> const_defs;
	vector<VarDef*> var_defs;
	vector<ProcDef*> proc_defs;
	vector<FuncDef*> func_defs;
	Block *block;
};

#endif /* COMPONENT_HXX */
