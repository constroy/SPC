/*
 * symtab.cxx
 *
 * Copyright 2015 constroy <constroy.li@gmail.com>
 *
 *
 */


#include "symtab.hxx"

SymTab::SymTab()
{
	p=0;
}
bool SymTab::push()
{
	if (p<STACK_SIZE) return p++;
	else return false;
}
bool SymTab::pop()
{
	return p?p--:p;
}
bool SymTab::insert(const std::string &name,const Symbol &symb)
{
	if (!p) return false;
	if (s[p-1].count(name)) return false;
	s[p-1][name]=symb;
	return true;
}
bool SymTab::find(const std::string &name,Symbol &symb) const
{
	if (!p) return false;
	for (int i=p-1;i>=0;--i) if (s[i].count(name))
	{
		symb=s[i].at(name);
		return true;
	}
	return false;
}
