/*
 * analyser.hpp
 *
 * Copyright 2015 constroy <constroy.li@gmail.com>
 *
 *
 */


#ifndef ANALYSER_HPP
#define ANALYSER_HPP

#include "component.hpp"
#include "symtab.hpp"

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

#endif /* ANALYSER_HPP */
