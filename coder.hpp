/*
 * coder.hpp
 *
 * Copyright 2015 constroy <constroy.li@gmail.com>
 *
 *
 */


#ifndef CODER_HPP
#define CODER_HPP

#include <string>
#include <array>
#include <vector>
#include <queue>
#include "lexer.hpp"
#include "symtab.hpp"
#include "optimizer.hpp"

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
		std::vector<std::string> data;
		std::vector<Instr> text;
		std::vector<TAC> tacs;
		std::queue<TAC> tacs2;
};

#endif /* CODER_HPP */
