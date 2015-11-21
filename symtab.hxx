/*
 * symtab.hxx
 *
 * Copyright 2015 constroy <constroy.li@gmail.com>
 *
 *
 */


#ifndef SYMTAB_HXX
#define SYMTAB_HXX

#include <string>
#include <unordered_map>

#define STACK_SIZE 256

enum SymKind{constant,variable,array,string,function,procedure};
enum SymType{None,integer,character};

struct Symbol
{
	SymKind kind;
	SymType type;
	int v;
};

class SymTab
{
	public:
		SymTab();
		bool push();
		bool pop();
		bool insert(const std::string &name,const Symbol &symb);
		bool find(const std::string &name,Symbol &symb) const;
	private:
		int p;
		std::unordered_map<std::string,Symbol> s[STACK_SIZE];
};

#endif /* SYMTAB_HXX */
