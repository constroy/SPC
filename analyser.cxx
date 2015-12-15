/*
 * analyser.cxx
 *
 * Copyright 2015 constroy <constroy.li@gmail.com>
 *
 *
 */


#include "analyser.hxx"

Analyser::Analyser(const Program *prog,const char file[]):
		program(prog),dst(fopen(file,"w")),coder(symtab)
{
	if (!dst)
	{
		error(std::string(file),open_failed);
		exit(1);
	}
}

void Analyser::genCode()
{
	symtab.push();
	program->scan(coder,symtab);
	if (success)
	{
		coder.unique();
		symtab.alloc();
		coder.append({"main:"});
		program->genCode(coder,symtab,"");
		/*
		coder.append({"mov","rax","60"});
		coder.append({"mov","rdi","0"});
		coder.append({"syscall"});
		*/
	}
	symtab.pop();
	if (success) fputs(coder.listCode().c_str(),dst);
}
