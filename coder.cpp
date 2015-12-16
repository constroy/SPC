/*
 * coder.cxx
 *
 * Copyright 2015 constroy <constroy.li@gmail.com>
 *
 *
 */


#include "coder.hxx"

Coder::Coder(SymTab &symtab): symtab(symtab) {}

int Coder::add(std::string str)
{
	int idx=data.size();
	data.push_back("db "+str+", 0h");
	return idx;
}

int Coder::getTextSize() const
{
	return text.size();
}

int Coder::append(Instr instr)
{
	int idx=text.size();
	text.push_back(instr);
	return idx;
}

void Coder::save_reg(const SymTab &symtab)
{
	for (auto &s:symtab.reg_symb) append({"mov","["+s.addr()+"]",s.reg});
}

void Coder::sync_reg(const SymTab &symtab)
{
	for (auto &s:symtab.reg_symb) append({"mov",s.reg,"["+s.addr()+"]"});
}

void Coder::push_reg(int n)
{
	for (int i=0;i<=n;++i) append({"push",base_reg[i]});
}

void Coder::pop_reg(int n)
{
	for (int i=n;i>=0;--i) append({"pop",base_reg[i]});
}

Instr &Coder::at(int idx)
{
	return text.at(idx);
}

TAC Coder::lastTAC()
{
	return tacs.back();
}

int Coder::putTAC(TAC tac)
{
	int idx=tacs.size();
	if (tac[1].type==ident && tac[1].s.empty())
		tac[1].s="#"+to_string(idx);
	tacs.push_back(tac);
	return idx;
}

//TODO:ref symb
void Coder::unique()
{
	Optimizer optim;
	Symbol symb={variable,int_t,INT_SIZE};
	for (int i=0;i<(int)tacs.size();++i) if (tacs[i][0].type!=semicolon)
	{
		tacs2.push(tacs[i]);
		//
		TAC tac=tacs[i];
		printf("unique: %d %s %s %s\n",tac[0].type,tac[1].s.c_str(),
				tac[2].s.c_str(),tac[3].s.c_str());
		//
		if (tacs[i][1].s=="#"+to_string(i))
		{
			symtab.insert(tacs[i][1].s,symb);
		}
		if (tacs[i][2].type==number)
		{
			Symbol c={constant,int_t,tacs[i][2].v};
			symtab.insert(tacs[i][2].s,c);
		}
		if (tacs[i][3].type==number)
		{
			Symbol c={constant,int_t,tacs[i][3].v};
			symtab.insert(tacs[i][3].s,c);
		}
		for (int j=1;j<4;++j) if (tacs[i][j].type)
		{
			symtab.refer(tacs[i][j].s);
		}
	}
	tacs.clear();
}

void Coder::transTAC(const TAC &tac)
{
	bool other;
	Symbol dest,arg0,arg1;
	symtab.find(tac[1].s,dest);
	symtab.find(tac[2].s,arg0);
	if ((other=tac[3].type))
	{
		symtab.find(tac[3].s,arg1);
	}
	else
	{
		arg1=arg0;
		symtab.find(tac[1].s,arg0);
	}
	switch (tac[0].type)
	{
		case plus:
		{
			if (other)
			{
				if (dest.reg.empty() && arg0.reg.empty())
				{
					append({"mov","cx",arg0.val()});
					append({"mov",dest.val(),"cx"});
				}
				else
				{
					append({"mov",dest.val(),arg0.val()});
				}
			}
			if (dest.reg.empty() && arg1.reg.empty())
			{
				append({"mov","cx",arg1.val()});
				append({"add",dest.val(),"cx"});
			}
			else
			{
				append({"add",dest.val(),arg1.val()});
			}
			break;
		}
		case minus:
		{
			if (other)
			{
				if (dest.reg.empty() && arg0.reg.empty())
				{
					append({"mov","cx",arg0.val()});
					append({"mov",dest.val(),"cx"});
				}
				else
				{
					append({"mov",dest.val(),arg0.val()});
				}
			}
			if (dest.reg.empty() && arg1.reg.empty())
			{
				append({"mov","cx",arg1.val()});
				append({"sub",dest.val(),"cx"});
			}
			else
			{
				append({"sub",dest.val(),arg1.val()});
			}
			break;
		}
		case times:
		{
			if (dest.reg.empty())
			{
				append({"mov","cx",arg0.val()});
				append({"imul","cx",arg1.val()});
				append({"mov",dest.val(),"cx"});
			}
			else
			{
				if (other) append({"mov",dest.val(),arg0.val()});
				append({"imul",dest.val(),arg1.val()});
			}
			break;
		}
		case slash:
		{
			append({"mov","cx",arg1.val()});
			if (arg0.val()!="ax") append({"mov","ax",arg0.val()});
			append({"cwd"});
			append({"idiv","cx"});
			append({"mov",dest.val(),"ax"});
			break;
		}
		case eql:
		{
			if (dest.reg.empty() && arg1.reg.empty())
			{
				append({"mov","cx",arg1.val()});
				append({"mov",dest.val(),"cx"});
			}
			else
			{
				append({"mov",dest.val(),arg1.val()});
			}
			break;
		}
		case lbracket:
		{
			append({"movsx","rcx",arg0.val()});
			if (arg1.reg.empty())
			{
				append({"mov","dx",arg1.val()});
				append({"mov",dest.val(),"dx"});
			}
			else
			{
				append({"mov",dest.val(),arg1.val()});
			}
			break;
		}
		case rbracket:
		{
			append({"movsx","rcx",arg1.val()});
			if (dest.reg.empty())
			{
				append({"mov","dx",arg0.val()});
				append({"mov",dest.val(),"dx"});
			}
			else
			{
				append({"mov",dest.val(),arg0.val()});
			}
			break;
		}
		default: break;
	}
}

void Coder::calc(const Token &res)
{
	printf("calc: %s\n",res.s.c_str());
	while (!tacs2.empty() && tacs2.front()[1]==res)
	{
		//
		TAC tac=tacs2.front();
		printf("calc: %d %s %s %s\n",tac[0].type,tac[1].s.c_str(),
				tac[2].s.c_str(),tac[3].s.c_str());
		//
		transTAC(tacs2.front());
		tacs2.pop();
	}
	puts("----------------------------");
}


std::string Coder::listCode() const
{
	std::string code="BITS 64\n";
	code+="extern scanf\n";
	code+="extern printf\n";
	code+="global main\n";
	code+="SECTION .data\n";
	for (int i=0;i<(int)data.size();++i)
	{
		code+="fmt"+to_string(i)+"\t"+data[i]+"\n";
	}
	code+="SECTION .text\n";
	std::string line;
	for (int i=0;i<(int)text.size();++i)
	{
		if (text[i][0].back()==':')
		{
			line.clear();
		}
		else
		{
			line="label_"+to_string(i)+":";
			line.resize(12,' ');
		}
		if (!text[i][0].empty()) line+=text[i][0];
		if (!text[i][1].empty()) line+=" "+text[i][1];
		if (!text[i][2].empty()) line+=", "+text[i][2];
		code+=line+"\n";
	}
	return code;
}
