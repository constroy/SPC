/*
 * symtab.cxx
 *
 * Copyright 2015 constroy <constroy.li@gmail.com>
 *
 *
 */


#include "symtab.hxx"

using std::to_string;

const char base_reg[BASE_REG_NUM][4]={"r8","r9","r10","r11","r12","r13","r14","r15"};
const char free_reg[FREE_REG_NUM][3]={"bx","si","di"};

SymTab::SymTab(): loop_level(0),tp(0) {}

std::string Symbol::addr() const
{
	switch (kind)
	{
		case variable:
		{
			return base_reg[level]+to_string(offset);
		}
		case array:
		{
			int scale=type==int_t?INT_SIZE:CHAR_SIZE;
			return base_reg[level]+("+rcx*"+to_string(scale))+to_string(offset);
		}
		case parameter:
		{
			return base_reg[level]+("+"+to_string(offset));
		}
		default: return "";
	}
}

std::string Symbol::val() const
{
	if (kind==constant) return to_string(v);
	else
	{
		if (reg.empty()) return "word ["+addr()+"]";
		else return reg;
	}
}

bool Symbol::operator <(const Symbol &symb) const
{
	return this->weight<symb.weight;
}

int SymTab::getLevel() const
{
	return tp-1;
}

int SymTab::localSize() const
{
	return -local_p;
}

void SymTab::push()
{
	if (tp<BASE_REG_NUM)
	{
		local_p=0;
		param_p=2*REG_SIZE;
		t[tp].clear();
		++tp;
	}
}

void SymTab::pop()
{
	tp && --tp;
}

bool SymTab::insert(const std::string &name,Symbol &symb)
{
	if (tp)
	{
		if (name!=to_string(symb.v) && t[tp-1].count(name))
		{
			error(name,redeclaration);
		}
		else
		{
			symb.level=getLevel();
			symb.weight=0;
			if (symb.kind==parameter)
			{
				symb.offset=param_p;
				param_p+=symb.size;
			}
			else if (symb.kind==function)
			{
				symb.reg="ax";
			}
			t[tp-1][name]=symb;
			return true;
		}
	}
	return false;
}

bool SymTab::find(const std::string &name,Symbol &symb) const
{
	if (tp)
	{
		for (int i=tp-1;i>=0;--i) if (t[i].count(name))
		{
			symb=t[i].at(name);
			if (i!=tp-1 && symb.kind!=function) symb.reg.clear();
			return true;
		}
		error(name,undefinition);
	}
	return false;
}

void SymTab::refer(const std::string &name)
{
	if (tp)
	{
		for (int i=tp-1;i>=0;--i)
		{
			if (t[i].count(name) && t[i][name].kind==variable)
			{
				t[i][name].weight+=1<<(loop_level+loop_level);
				break;
			}
		}
	}
}

void SymTab::alloc()
{
	for (auto &i:t[tp-1])
	{
		auto &symb=i.second;
		if (symb.kind==variable || symb.kind==array)
		{
			symb.offset=local_p-=symb.size;
			//
			printf("alloc: mem %d to: %s\n",symb.offset,i.first.c_str());
			//
		}
	}

	reg_symb.clear();
	for (int i=0;i<FREE_REG_NUM;++i)
	{
		auto it=std::max_element(t[tp-1].begin(),t[tp-1].end(),
				[](const auto &a,const auto &b){return a.second<b.second;});
		if (it!=t[tp-1].end() && it->second.weight)
		{
			it->second.weight=0;
			it->second.reg=free_reg[i];
			reg_symb.push_back(it->second);
			//
			printf("alloc: reg %s to: %s\n",free_reg[i],it->first.c_str());
			//
		}
		else
		{
			break;
		}
	}
}
