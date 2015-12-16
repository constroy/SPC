/*
 * analyser.hxx
 *
 * Copyright 2015 constroy <constroy.li@gmail.com>
 *
 *
 */


#ifndef ANALYSER_HXX
#define ANALYSER_HXX

#include "component.hxx"
#include "symtab.hxx"

class Analyser
{
	public:
		Analyser(const Program *prog,const char file[]);
		void genCode();
	private:
		const Program *program;
		FILE *dst;
		SymTab symtab;
		Coder coder;
};

#endif /* ANALYSER_HXX */
