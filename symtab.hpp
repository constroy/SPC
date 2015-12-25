/*
 * symtab.hpp
 *
 * Copyright 2015 constroy <constroy.li@gmail.com>
 *
 *
 */


#ifndef SYMTAB_HPP
#define SYMTAB_HPP

#include <vector>
#include <string>
#include <algorithm>
#include <unordered_map>
#include "error.hpp"

#define BASE_REG_NUM 8
#define FREE_REG_NUM 3
#define REG_SIZE 8
#define INT_SIZE 2
#define CHAR_SIZE 2

enum SymKind{constant,variable,array,parameter,procedure,function};
enum SymType{void_t,int_t,char_t};

extern const char base_reg[BASE_REG_NUM][4];
extern const char free_reg[FREE_REG_NUM][3];

struct Symbol
{
	SymKind kind;
	SymType type;
	union {int v,size;};
	int level,weight,offset;
	std::vector<bool> ref_para;
	std::string reg;
	std::string addr() const;
	std::string val() const;
	bool operator <(const Symbol &symb) const;
};

class SymTab
{
	public:
		SymTab();
		int getLevel() const;
		std::string getName(int l) const;
		int localSize() const;
		void push(const std::string &s);
		void pop();
		bool insert(const std::string &name,Symbol &symb);
		bool find(const std::string &name,Symbol &symb) const;
		void refer(const std::string &name);
		void alloc();
		int loop_level;
		std::vector<Symbol> reg_symb;
	private:
		int tp;
		int local_p,param_p;
		std::string names[BASE_REG_NUM];
		std::unordered_map<std::string,Symbol> t[BASE_REG_NUM];
};

#endif /* SYMTAB_HPP */
