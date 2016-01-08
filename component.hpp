/*
 * parser.hpp
 *
 * Copyright 2015 constroy <constroy.li@gmail.com>
 *
 *
 */


#ifndef COMPONENT_HPP
#define COMPONENT_HPP

#include <utility>
#include <vector>
#include "lexer.hpp"
#include "coder.hpp"
#include "symtab.hpp"

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
	std::vector<Token*> names;
	int size;
	Token *type;
};

struct ParaDef
{
	ParaDef(Lexer &lexer);
	~ParaDef();
	void scan(Coder &coder,SymTab &symtab) const;
	std::vector<Token*> names;
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
	std::vector<std::pair<bool,ParaDef*>> para_list;
	Program *program;
};

struct FuncDef
{
	FuncDef(Lexer &lexer);
	~FuncDef();
	void scan(Coder &coder,SymTab &symtab) const;
	void genCode(Coder &coder,SymTab &symtab) const;
	Token *name,*type;
	std::vector<std::pair<bool,ParaDef*>> para_list;
	Program *program;
};

struct Expression;

struct Factor
{
	Factor(Lexer &lexer);
	~Factor();
	const Factor *getFactor() const;
	void scan(Coder &coder,SymTab &symtab);
	void genCode(Coder &coder,SymTab &symtab) const;
	Token *token;
	Expression *exp;
	std::vector<Expression*> arg_list;
	int i;
	Token res;
};

struct Term
{
	Term(Lexer &lexer);
	~Term();
	const Factor *getFactor() const;
	void scan(Coder &coder,SymTab &symtab);
	void genCode(Coder &coder,SymTab &symtab) const;
	std::vector<std::pair<Token*,Factor*>> factors;
	Token res;
};

struct Expression
{
	Expression(Lexer &lexer);
	~Expression();
	const Factor *getFactor() const;
	void scan(Coder &coder,SymTab &symtab);
	void genCode(Coder &coder,SymTab &symtab) const;
	std::vector<std::pair<Token*,Term*>> terms;
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
	std::vector<Expression*> arg_list;
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
	std::vector<Token*> tokens;
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
	std::vector<Statement*> statements;
};
struct Program
{
	Program(Lexer &Lexer);
	~Program();
	void scan(Coder &coder,SymTab &symtab) const;
	void genCode(Coder &coder,SymTab &symtab,const std::string &val) const;
	std::vector<ConstDef*> const_defs;
	std::vector<VarDef*> var_defs;
	std::vector<ProcDef*> proc_defs;
	std::vector<FuncDef*> func_defs;
	Block *block;
};

#endif /* COMPONENT_HPP */
