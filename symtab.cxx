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
	if (p<STACK_SIZE)
	{
		s[p]=s[p-1];
		++p;
		return true;
	}
	else
	{
		return false;
	}
}
bool SymTab::pop()
{
	return p?p--:p;
}
bool SymTab::insert(const std::string &name,Symbol &symb)
{
	if ((symb.l=p))
	{
		s[p-1][name]=symb;
		return true;
	}
	else
	{
		return false;
	}
}
int SymTab::level(const std::string &name) const
{
	if (p && s[p-1].count(name)) return s[p-1].at(name).l;
	else return 0;
}
Symbol SymTab::at(const std::string &name) const
{
	return s[p-1].at(name);
}
