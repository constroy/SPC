/*
 * optimizer.h
 *
 * Copyright 2015 constroy <constroy.li@gmail.com>
 *
 *
 */


#ifndef OPTIMIZER_HXX
#define OPTIMIZER_HXX

#include <array>
#include <vector>
#include "lexer.hxx"
#include "symtab.hxx"

using std::vector;

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
		vector<TAC> tacs;
};

#endif /* OPTIMIZER_HXX */
