/*
 * coder.hxx
 *
 * Copyright 2015 constroy <constroy.li@gmail.com>
 *
 *
 */


#ifndef CODER_HXX
#define CODER_HXX

#include <string>
#include <array>
#include <vector>
#include <queue>
#include "lexer.hxx"
#include "symtab.hxx"
#include "optimizer.hxx"

using std::vector;
using std::queue;
using std::to_string;

typedef std::array<std::string,3> Instr;

class Coder
{
	public:
		Coder(SymTab &symtab);
		int add(std::string fmt);
		int getTextSize() const;
		int append(Instr instr);
		void save_reg(const SymTab &symtab);
		void sync_reg(const SymTab &symtab);
		void push_reg(int n);
		void pop_reg(int n);
		Instr &at(int idx);
		TAC lastTAC();
		int putTAC(TAC tac);
		void unique();
		void calc(const Token &res);
		std::string listCode() const;
	private:
		void transTAC(const TAC &tac);
		SymTab &symtab;
		vector<std::string> data;
		vector<Instr> text;
		vector<TAC> tacs;
		queue<TAC> tacs2;
};

#endif /* CODER_HXX */
