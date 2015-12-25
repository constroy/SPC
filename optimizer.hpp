/*
 * optimizer.h
 *
 * Copyright 2015 constroy <constroy.li@gmail.com>
 *
 *
 */


#ifndef OPTIMIZER_HPP
#define OPTIMIZER_HPP

#include <array>
#include <vector>
#include "lexer.hpp"
#include "symtab.hpp"

/*
 * Three-addests code (op,dest,arg0,arg1)
 * + | dest := arg0 + arg1
 * - | dest := arg0 - arg1
 * * | dest := arg0 * arg1
 * / | dest := arg0 / arg1
 * + | dest := dest + arg0
 * - | dest := dest - arg0
 * * | dest := dest * arg0
 * / | dest := dest / arg0
 * = | dest := arg0
 * [ | dest[arg0] := arg1
 * ] | dest := arg0[arg1]
 * ; | separator
 */

typedef std::array<Token,4> TAC;

class Optimizer
{
	public:
		void optimize();
	private:
		std::vector<TAC> tacs;
};

#endif /* OPTIMIZER_HPP */
