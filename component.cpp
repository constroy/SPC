/*
 * parser.hxx
 *
 * Copyright 2015 constroy <constroy.li@gmail.com>
 *
 *
 */


#include "component.hxx"

ConstDef::ConstDef(Lexer &lexer): name(nullptr),value(nullptr)
{
	if (lexer.currToken().type==ident)
	{
		name=new Token(lexer.currToken());
		if (lexer.nextToken().type!=eql) warning(lexer,lost_eql);
		Token token=lexer.nextToken();
		if (token.type==plus || token.type==minus)
		{
			if (lexer.nextToken().type==number)
			{
				value=new Token(lexer.currToken());
				if (token.type==minus) value->v=-value->v;
				lexer.nextToken();
			}
			else
			{
				error(lexer,lost_number);
			}
		}
		else if (token.type==number || token.type==character)
		{
			value=new Token(token);
			lexer.nextToken();
		}
		else
		{
			error(lexer,lost_num_or_char);
		}
	}
	else
	{
		error(lexer,lost_ident);
	}
}

ConstDef::~ConstDef()
{
	delete name;
	delete value;
}

void ConstDef::scan(Coder &coder,SymTab &symtab) const
{
	Symbol symb={constant,value->type==number?int_t:char_t,value->v};
	symtab.insert(name->s,symb);
}

VarDef::VarDef(Lexer &lexer): size(0),type(nullptr)
{
	names.push_back(new Token(lexer.currToken()));
	while (lexer.nextToken().type==comma)
		names.push_back(new Token(lexer.nextToken()));
	if (lexer.currToken().type==colon)
	{
		if (lexer.nextToken().type==word_array)
		{
			if (lexer.nextToken().type==lbracket)
			{
				if (lexer.nextToken().type==number)
				{
					size=lexer.currToken().v;
					if (lexer.nextToken().type==rbracket) lexer.nextToken();
					else warning(lexer,lost_rbracket);
					if (lexer.currToken().type==word_of) lexer.nextToken();
					else warning(lexer,lost_of);
				}
				else
				{
					error(lexer,lost_number);
				}
			}
			else
			{
				error(lexer,lost_lbracket);
			}
		}
		if (lexer.currToken().type==word_integer ||
				lexer.currToken().type==word_char)
		{
			type=new Token(lexer.currToken());
			if (lexer.nextToken().type==semicolon) lexer.nextToken();
			else warning(lexer,lost_semicolon);
		}
		else
		{
			error(lexer,lost_type);
		}
	}
	else
	{
		error(lexer,lost_colon);
	}
}

VarDef::~VarDef()
{
	for (const auto &i:names) delete i;
	delete type;
}

void VarDef::scan(Coder &coder,SymTab &symtab) const
{
	Symbol symb;
	if (size)
	{
		symb.kind=array;
		symb.type=type->type==word_integer?int_t:char_t;
		symb.size=size*(type->type==word_integer?INT_SIZE:CHAR_SIZE);
	}
	else
	{
		symb.kind=variable;
		symb.type=type->type==word_integer?int_t:char_t;
		symb.size=type->type==word_integer?INT_SIZE:CHAR_SIZE;
	}
	for (const auto &i:names) symtab.insert(i->s,symb);
}

ParaDef::ParaDef(Lexer &lexer): size(0),type(nullptr)
{
	names.push_back(new Token(lexer.currToken()));
	while (lexer.nextToken().type==comma)
		names.push_back(new Token(lexer.nextToken()));
	if (lexer.currToken().type==colon)
	{
		if (lexer.nextToken().type==word_array)
		{
			if (lexer.nextToken().type==lbracket)
			{
				if (lexer.nextToken().type==number)
				{
					size=lexer.currToken().v;
					if (lexer.nextToken().type==rbracket) lexer.nextToken();
					else warning(lexer,lost_rbracket);
					if (lexer.currToken().type==word_of) lexer.nextToken();
					else warning(lexer,lost_of);
				}
				else
				{
					error(lexer,lost_number);
				}
			}
			else
			{
				error(lexer,lost_lbracket);
			}
		}
		if (lexer.currToken().type==word_integer ||
				lexer.currToken().type==word_char)
		{
			type=new Token(lexer.currToken());
			lexer.nextToken();
		}
		else
		{
			error(lexer,lost_type);
		}
	}
	else
	{
		error(lexer,lost_colon);
	}
}

ParaDef::~ParaDef()
{
	for (const auto &i:names) delete i;
	delete type;
}

void ParaDef::scan(Coder &coder,SymTab &symtab) const
{
	Symbol symb={parameter,
			type->type==word_integer?int_t:char_t,
			type->type==word_integer?INT_SIZE:CHAR_SIZE};
	for (const auto &i:names) symtab.insert(i->s,symb);
}

ProcDef::ProcDef(Lexer &lexer): name(new Token(lexer.currToken())),
		program(nullptr)
{
	if (lexer.nextToken().type==lparen)
	{
		bool var;
		if ((var=lexer.nextToken().type==word_var))
			lexer.nextToken();
		para_list.push_back(make_pair(var,new ParaDef(lexer)));
		while (lexer.currToken().type==semicolon)
		{
			if ((var=lexer.nextToken().type==word_var))
				lexer.nextToken();
			para_list.push_back(make_pair(var,new ParaDef(lexer)));
		}
		if (lexer.currToken().type==rparen) lexer.nextToken();
		else warning(lexer,lost_rparen);
		if (lexer.currToken().type==semicolon) lexer.nextToken();
		else warning(lexer,lost_semicolon);
	}
	program=new Program(lexer);
	if (lexer.currToken().type==semicolon) lexer.nextToken();
	else warning(lexer,lost_semicolon);
}

ProcDef::~ProcDef()
{
	delete name;
	for (const auto &i:para_list) delete i.second;
	delete program;
}

void ProcDef::scan(Coder &coder,SymTab &symtab) const
{
	Symbol symb={procedure,void_t};
	symb.ref_para.reserve(para_list.size());
	for (const auto &i:para_list)
	{
		for (int j=0;j<(int)i.second->names.size();++j)
			symb.ref_para.push_back(i.first);
	}
	symtab.insert(name->s,symb);
}

void ProcDef::genCode(Coder &coder,SymTab &symtab) const
{
	symtab.push();
	for (auto const &i:para_list) i.second->scan(coder,symtab);
	program->scan(coder,symtab);
	if (success)
	{
		coder.unique();
		symtab.alloc();
		coder.append({name->s+":"});
		program->genCode(coder,symtab,"");
	}
	symtab.pop();

}

FuncDef::FuncDef(Lexer &lexer): name(new Token(lexer.currToken())),
		type(nullptr),program(nullptr)
{
	if (lexer.nextToken().type==lparen)
	{
		bool var;
		if ((var=lexer.nextToken().type==word_var))
			lexer.nextToken();
		para_list.push_back(make_pair(var,new ParaDef(lexer)));
		while (lexer.currToken().type==semicolon)
		{
			if ((var=lexer.nextToken().type==word_var))
				lexer.nextToken();
			para_list.push_back(make_pair(var,new ParaDef(lexer)));
		}
		if (lexer.currToken().type==rparen) lexer.nextToken();
		else warning(lexer,lost_rparen);
	}
	if (lexer.currToken().type==colon) lexer.nextToken();
	else error(lexer,lost_colon);
	if (lexer.currToken().type==word_integer ||
			lexer.currToken().type==word_char)
	{
		type=new Token(lexer.currToken());
		lexer.nextToken();
	}
	else error(lexer,lost_type);
	if (lexer.currToken().type==semicolon) lexer.nextToken();
	else warning(lexer,lost_semicolon);
	program=new Program(lexer);
	if (lexer.currToken().type==semicolon) lexer.nextToken();
	else warning(lexer,lost_semicolon);
}

FuncDef::~FuncDef()
{
	delete name;
	delete type;
	for (const auto &i:para_list) delete i.second;
	delete program;
}

void FuncDef::scan(Coder &coder,SymTab &symtab) const
{
	Symbol symb={function,type->type==word_integer?int_t:char_t};
	symb.ref_para.reserve(para_list.size());
	for (const auto &i:para_list)
	{
		for (int j=0;j<(int)i.second->names.size();++j)
			symb.ref_para.push_back(i.first);
	}
	symtab.insert(name->s+"@",symb);
}

void FuncDef::genCode(Coder &coder,SymTab &symtab) const
{
	symtab.push();
	for (auto const &i:para_list) i.second->scan(coder,symtab);
	Symbol symb={variable,
			type->type==word_integer?int_t:char_t,
			type->type==word_integer?INT_SIZE:CHAR_SIZE};
	symtab.insert(name->s,symb);
	program->scan(coder,symtab);
	if (success)
	{
		coder.unique();
		symtab.alloc();
		coder.append({name->s+"@:"});
		symtab.find(name->s,symb);
		program->genCode(coder,symtab,symb.val());
	}
	symtab.pop();
}

Factor::Factor(Lexer &lexer): token(nullptr),exp(nullptr)
{
	if (lexer.currToken().type==lparen)
	{
		lexer.nextToken();
		exp=new Expression(lexer);
		if (lexer.currToken().type==rparen) lexer.nextToken();
		else error(lexer,lost_rparen);
	}
	else
	{
		token=new Token(lexer.currToken());
		if (token->type==ident || token->type==number)
		{
			lexer.nextToken();
			if (lexer.currToken().type==lbracket)
			{
				lexer.nextToken();
				exp=new Expression(lexer);
				if (lexer.currToken().type==rbracket) lexer.nextToken();
				else warning(lexer,lost_rbracket);
			}
			else if (lexer.currToken().type==lparen)
			{
				token->s+="@";
				lexer.nextToken();
				arg_list.push_back(new Expression(lexer));
				while (lexer.currToken().type==comma)
				{
					lexer.nextToken();
					arg_list.push_back(new Expression(lexer));
				}
				if (lexer.currToken().type==rparen) lexer.nextToken();
				else warning(lexer,lost_rparen);
			}
		}
		else
		{
			error(lexer,not_value);
		}
	}
}

Factor::~Factor()
{
	delete token;
	delete exp;
	for (const auto &i:arg_list) delete i;
}

void Factor::scan(Coder &coder,SymTab &symtab)
{
	if (token)
	{
		Symbol symb;
		if (token->type==number)
		{
			symb.kind=constant;
			symb.type=int_t;
			symb.v=token->v;
			symtab.insert(token->s,symb);
		}
		if (symtab.find(token->s,symb))
		{
			if (exp)
			{
				if (symb.kind==array) exp->scan(coder,symtab);
				else error(token->s,not_array);
				Token op={rbracket};
				Token dest={ident};
				Token arg1=coder.lastTAC()[1];
				coder.putTAC({op,dest,*token,arg1});
				res=coder.lastTAC()[1];
			}
			else
			{
				if (symb.kind==constant ||
					symb.kind==variable ||
					symb.kind==parameter)
				{
					if (arg_list.size()) error(token->s,cannot_call);
				}
				else if (symb.kind==function)
				{
					if (symb.ref_para.size()==arg_list.size())
					{
						for (int i=symb.ref_para.size()-1;i>=0;--i)
						{
							if (symb.ref_para[i] &&
									!arg_list[i]->isVar(symtab))
							{
								error(token->s,ref_rvalue);
								break;
							}
							arg_list[i]->scan(coder,symtab);
						}
						coder.putTAC({semicolon});
					}
					else
					{
						error(token->s,wrong_arg_num);
					}
				}
				else if (symb.kind==array)
				{
					error(token->s,lost_subscript);
				}
				else if (symb.kind==procedure)
				{
					error(token->s,not_value);
				}
				res=*token;
			}
		}
	}
	else if (exp)
	{
		exp->scan(coder,symtab);
		res=exp->res;
	}
}

void Factor::genCode(Coder &coder,SymTab &symtab) const
{
	if (token)
	{
		Symbol symb;
		symtab.find(token->s,symb);
		if (exp)
		{
			exp->genCode(coder,symtab);
			coder.calc(res);
		}
		else
		{
			if (symb.kind==constant ||
				symb.kind==variable ||
				symb.kind==parameter)
			{
				//nothing
			}
			else if (symb.kind==function)
			{
				Symbol arg;
				for (int i=arg_list.size()-1;i>=0;--i)
				{
					arg_list[i]->genCode(coder,symtab);
					if (symtab.find(arg_list[i]->res.s,arg)) coder.append({"push",arg.val()});
				}
				coder.save_reg(symtab);
				coder.append({"call",token->s});
				coder.sync_reg(symtab);
				for (int i=0;i<(int)symb.ref_para.size();++i)
				{
					if (symb.ref_para[i])
					{
						coder.append({"pop",arg_list[i]->getVar(symtab).val()});
					}
					else
					{
						coder.append({"add","rsp",to_string(INT_SIZE)});
					}
				}
			}
		}
	}
	else if (exp)
	{
		exp->genCode(coder,symtab);
	}
}

Term::Term(Lexer &lexer)
{
	Token *token=new Token{times,"*"};
	factors.push_back(make_pair(token,new Factor(lexer)));
	while (lexer.currToken().type==times || lexer.currToken().type==slash)
	{
		token=new Token(lexer.currToken());
		lexer.nextToken();
		factors.push_back(make_pair(token,new Factor(lexer)));
	}
}

Term::~Term()
{
	for (const auto &i:factors) delete i.first,delete i.second;
}

bool Term::isVar(const SymTab &symtab) const
{
	if (factors.size()==1)
	{
		Symbol symb;
		if (symtab.find(factors[0].second->token->s,symb))
		{
			return symb.kind==variable;
		}
	}
	return false;
}

Symbol Term::getVar(const SymTab &symtab) const
{
	Symbol symb;
	symtab.find(factors[0].second->token->s,symb);
	return symb;
}

void Term::scan(Coder &coder,SymTab &symtab)
{
	Token op={eql};
	Token dest={ident};
	Token arg0={number,"1",1};
	coder.putTAC({op,dest,arg0});
	res=coder.lastTAC()[1];
	for (auto fact:factors)
	{
		fact.second->scan(coder,symtab);
		coder.putTAC({*fact.first,res,fact.second->res});
	}
}

void Term::genCode(Coder &coder,SymTab &symtab) const
{
	coder.calc(res);
	for (const auto &fact:factors)
	{
		fact.second->genCode(coder,symtab);
		coder.calc(res);
	}
}

Expression::Expression(Lexer &lexer)
{
	Token *token=new Token(lexer.currToken());
	if (token->type==plus || token->type==minus)
	{
		lexer.nextToken();
	}
	else
	{
		token->type=plus;
		token->s="+";
	}
	terms.push_back(make_pair(token,new Term(lexer)));
	while (lexer.currToken().type==plus || lexer.currToken().type==minus)
	{
		token=new Token(lexer.currToken());
		lexer.nextToken();
		terms.push_back(make_pair(token,new Term(lexer)));
	}
}

Expression::~Expression()
{
	for (const auto &i:terms) delete i.first,delete i.second;
}

bool Expression::isVar(const SymTab &symtab) const
{
	return terms.size()==1 && terms[0].second->isVar(symtab);
}

Symbol Expression::getVar(const SymTab &symtab) const
{
	return terms[0].second->getVar(symtab);
}

void Expression::scan(Coder &coder,SymTab &symtab)
{
	Token op={eql};
	Token dest={ident};
	Token arg0={number,"0",0};
	coder.putTAC({op,dest,arg0});
	res=coder.lastTAC()[1];
	for (auto term:terms)
	{
		term.second->scan(coder,symtab);
		coder.putTAC({*term.first,res,term.second->res});
	}
}

void Expression::genCode(Coder &coder,SymTab &symtab) const
{
	coder.calc(res);
	for (const auto &term:terms)
	{
		term.second->genCode(coder,symtab);
		coder.calc(res);
	}
}

Condition::Condition(Lexer &lexer): token(nullptr),exp0(nullptr),exp1(nullptr)
{
	exp0=new Expression(lexer);
	const Token temp=lexer.currToken();
	if (eql<=temp.type && temp.type<=gtr)
	{
		token=new Token(temp);
		lexer.nextToken();
		exp1=new Expression(lexer);
	}
	else
	{
		error(lexer,lost_relational);
	}
}

Condition::~Condition()
{
	delete token;
	delete exp0;
	delete exp1;
}

void Condition::scan(Coder &coder,SymTab &symtab)
{
	exp0->scan(coder,symtab);
	exp1->scan(coder,symtab);
	coder.putTAC({semicolon});
}

void Condition::genCode(Coder &coder,SymTab &symtab,bool res) const
{
	exp0->genCode(coder,symtab);
	exp1->genCode(coder,symtab);
	Symbol s0,s1;
	symtab.find(exp0->res.s,s0);
	symtab.find(exp1->res.s,s1);
	if (s0.reg.empty())
	{
		coder.append({"mov","ax",s0.val()});
		coder.append({"cmp","ax",s1.val()});
	}
	else
	{
		coder.append({"cmp",s0.val(),s1.val()});
	}
	switch (token->type^!res)
	{
		case eql: coder.append({"je"});break;
		case neq: coder.append({"jne"});break;
		case lss: coder.append({"jl"});break;
		case geq: coder.append({"jge"});break;
		case leq: coder.append({"jle"});break;
		case gtr: coder.append({"jg"});break;
		default: break;
	}
}

Statement *Statement::bear(Lexer &lexer)
{
	const Token token=lexer.currToken();
	switch (token.type)
	{
		case ident:
		{
			if (lexer.nextToken().type==lbracket)
			{
				lexer.nextToken();
				Expression *exp=new Expression(lexer);
				if (lexer.currToken().type==rbracket) lexer.nextToken();
				else warning(lexer,lost_rbracket);
				if (lexer.currToken().type==assign)
				{
					lexer.nextToken();
					return new Assignment(token,exp,lexer);
				}
				else
				{
					error(lexer,lost_assign);
				}
			}
			else if (lexer.currToken().type==assign)
			{
				lexer.nextToken();
				return new Assignment(token,nullptr,lexer);
			}
			else
			{
				return new ProcCall(token,lexer);
			}
			break;
		}
		case word_do:
		{
			lexer.nextToken();
			return new DoWhile(lexer);
			break;
		}
		case word_for:
		{
			lexer.nextToken();
			return new ForDo(lexer);
			break;
		}
		case word_if:
		{
			lexer.nextToken();
			return new IfThen(lexer);
			break;
		}
		case word_read:
		{
			lexer.nextToken();
			return new Read(lexer);
			break;
		}
		case word_write:
		{
			lexer.nextToken();
			return new Write(lexer);
			break;
		}
		case word_begin:
		{
			lexer.nextToken();
			return new Block(lexer);
			break;
		}
		default:
		{
			warning(lexer,empty_statement);
			break;
		}
	}
	return nullptr;
}

Statement::~Statement() {}

Assignment::Assignment(Token token,Expression *exp,Lexer &lexer):
		dest(new Token(token)),exp0(exp),exp1(new Expression(lexer)) {}

Assignment::~Assignment()
{
	delete dest;
	delete exp0,
	delete exp1;
}

void Assignment::scan(Coder &coder,SymTab &symtab)
{
	exp1->scan(coder,symtab);
	if (exp0)
	{
		exp0->scan(coder,symtab);
		Token op={lbracket};
		coder.putTAC({op,*dest,exp0->res,exp1->res});
	}
	else
	{
		Token op={eql};
		coder.putTAC({op,*dest,exp1->res});
	}

}

void Assignment::genCode(Coder &coder,SymTab &symtab) const
{
	exp1->genCode(coder,symtab);
	if (exp0) exp0->genCode(coder,symtab);
	coder.calc(*dest);
}

ProcCall::ProcCall(Token token,Lexer &lexer): name(new Token(token))
{
	if (lexer.currToken().type==lparen && lexer.nextToken().type!=rparen)
	{
		arg_list.push_back(new Expression(lexer));
		while (lexer.currToken().type==comma)
		{
			lexer.nextToken();
			arg_list.push_back(new Expression(lexer));
		}
		if (lexer.currToken().type==rparen) lexer.nextToken();
		else warning(lexer,lost_rparen);
	}
}

ProcCall::~ProcCall()
{
	delete name;
	for (const auto &i:arg_list) delete i;
}

void ProcCall::scan(Coder &coder,SymTab &symtab)
{
	Symbol symb;
	if (symtab.find(name->s,symb))
	{
		if (symb.kind==procedure)
		{
			if (symb.ref_para.size()==arg_list.size())
			{
				for (int i=symb.ref_para.size()-1;i>=0;--i)
				{
					if (symb.ref_para[i] &&
							!arg_list[i]->isVar(symtab))
					{
						error(name->s,ref_rvalue);
						break;
					}
					arg_list[i]->scan(coder,symtab);
				}
			}
			else
			{
				error(name->s,wrong_arg_num);
			}
		}
		else
		{
			error(name->s,cannot_call);
		}
	}
	coder.putTAC({semicolon});
}

void ProcCall::genCode(Coder &coder,SymTab &symtab) const
{
	Symbol symb,arg;
	symtab.find(name->s,symb);
	for (int i=arg_list.size()-1;i>=0;--i)
	{
		arg_list[i]->genCode(coder,symtab);
		if (symtab.find(arg_list[i]->res.s,arg)) coder.append({"push",arg.val()});
	}
	coder.save_reg(symtab);
	coder.append({"call",name->s});
	coder.sync_reg(symtab);
	for (int i=0;i<(int)symb.ref_para.size();++i)
	{
		if (symb.ref_para[i])
		{
			coder.append({"pop",arg_list[i]->getVar(symtab).val()});
		}
		else
		{
			coder.append({"add","rsp",to_string(INT_SIZE)});
		}
	}
}

DoWhile::DoWhile(Lexer &lexer): statement(nullptr),condition(nullptr)
{
	statement=Statement::bear(lexer);
	if (lexer.currToken().type==word_while)
	{
		lexer.nextToken();
		condition=new Condition(lexer);
	}
	else
	{
		error(lexer,lost_while);
	}
}

DoWhile::~DoWhile()
{
	delete condition;
	delete statement;
}

void DoWhile::scan(Coder &coder,SymTab &symtab)
{
	if (statement) statement->scan(coder,symtab);
	condition->scan(coder,symtab);
	coder.putTAC({semicolon});
}

void DoWhile::genCode(Coder &coder,SymTab &symtab) const
{
	int i0=coder.getTextSize();
	if (statement) statement->genCode(coder,symtab);
	condition->genCode(coder,symtab,true);
	coder.at(coder.getTextSize()-1)[1]="label_"+to_string(i0);
}

ForDo::ForDo(Lexer &lexer): token0(nullptr),token1(nullptr),
		exp0(nullptr),exp1(nullptr),statement(nullptr)
{
	token0=new Token(lexer.currToken());
	if (lexer.nextToken().type==assign)
	{
		lexer.nextToken();
		exp0=new Expression(lexer);
		if (lexer.currToken().type==word_to ||
			lexer.currToken().type==word_downto)
		{
			token1=new Token(lexer.currToken());
			lexer.nextToken();
			exp1=new Expression(lexer);
			if (lexer.currToken().type==word_do)
			{
				lexer.nextToken();
				statement=Statement::bear(lexer);
			}
			else
			{
				error(lexer,lost_do);
			}
		}
		else
		{
			error(lexer,lost_to);
		}
	}
	else
	{
		error(lexer,lost_assign);
	}
}

ForDo::~ForDo()
{
	delete token0;
	delete token1;
	delete exp0;
	delete exp1;
	delete statement;
}

void ForDo::scan(Coder &coder,SymTab &symtab)
{
	exp0->scan(coder,symtab);
	Token op={eql};
	coder.putTAC({op,*token0,exp0->res});
	exp1->scan(coder,symtab);
	statement->scan(coder,symtab);
	coder.putTAC({semicolon});
}

void ForDo::genCode(Coder &coder,SymTab &symtab) const
{
	exp0->genCode(coder,symtab);
	coder.calc(*token0);
	exp1->genCode(coder,symtab);
	int i0,i1;
	Symbol iter,dest;
	symtab.find(token0->s,iter);
	symtab.find(exp1->res.s,dest);

	if (iter.reg.empty() && dest.reg.empty())
	{
		i0=coder.append({"mov","cx",dest.val()});
		coder.append({"cmp",iter.val(),"cx"});
	}
	else
	{
		i0=coder.append({"cmp",iter.val(),dest.val()});
	}
	i1=coder.append({token1->type==word_to?"jg":"jl"});
	statement->genCode(coder,symtab);
	coder.append({token1->type==word_to?"inc":"dec",iter.val()});
	coder.append({"jmp","label_"+to_string(i0)});
	coder.at(i1)[1]="label_"+to_string(coder.getTextSize());
}

IfThen::IfThen(Lexer &lexer): condition(nullptr),statement0(nullptr),
		statement1(nullptr)
{
	condition=new Condition(lexer);
	if (lexer.currToken().type==word_then)
	{
		lexer.nextToken();
		statement0=Statement::bear(lexer);
		if (lexer.currToken().type==word_else)
		{
			lexer.nextToken();
			statement1=Statement::bear(lexer);
		}
	}
	else
	{
		error(lexer,lost_then);
	}
}

IfThen::~IfThen()
{
	delete condition;
	delete statement0;
	delete statement1;
}

void IfThen::scan(Coder &coder,SymTab &symtab)
{
	condition->scan(coder,symtab);
	if (statement0)
	{
		statement0->scan(coder,symtab);
		coder.putTAC({semicolon});
	}
	if (statement1)
	{
		statement1->scan(coder,symtab);
		coder.putTAC({semicolon});
	}
}

void IfThen::genCode(Coder &coder,SymTab &symtab) const
{
	condition->genCode(coder,symtab,false);
	int i0=coder.getTextSize()-1;
	if (statement0) statement0->genCode(coder,symtab);
	int i1=coder.append({"jmp"});
	coder.at(i0)[1]="label_"+to_string(i1+1);
	if (statement1) statement1->genCode(coder,symtab);
	coder.at(i1)[1]="label_"+to_string(coder.getTextSize());
}

Read::Read(Lexer &lexer)
{
	if (lexer.currToken().type==lparen)
	{
		do
		{
			tokens.push_back(new Token(lexer.nextToken()));
		}
		while (lexer.nextToken().type==comma);
		if (lexer.currToken().type==rparen) lexer.nextToken();
		else warning(lexer,lost_rparen);
	}
	else
	{
		error(lexer,lost_lparen);
	}
}

Read::~Read()
{
	for (const auto &i:tokens) delete i;
}

void Read::scan(Coder &coder,SymTab &symtab)
{
	Symbol symb;
	for (auto &i:tokens) if (symtab.find(i->s,symb))
	{
		if (symb.kind==variable) symtab.refer(i->s);
		else error(i->s,cannot_read);
	}
	coder.putTAC({semicolon});
}

void Read::genCode(Coder &coder,SymTab &symtab) const
{
	Symbol symb;
	for (auto &i:tokens) if (symtab.find(i->s,symb))
	{
		std::string fmt;
		if (symb.type==int_t) fmt="'%hd'";
		if (symb.type==char_t) fmt="'%c'";
		coder.save_reg(symtab);
		coder.push_reg(symtab.getLevel());
		coder.append({"mov","rdi","fmt"+to_string(coder.add(fmt))});
		coder.append({"lea","rsi","["+symb.addr()+"]"});
		coder.append({"xor","rax","rax"});
		coder.append({"call","scanf"});
		coder.pop_reg(symtab.getLevel());
		coder.sync_reg(symtab);
	}
}

Write::Write(Lexer &lexer): token(nullptr),exp(nullptr)
{
	if (lexer.currToken().type==lparen)
	{
		if (lexer.nextToken().type==string)
		{
			token=new Token(lexer.currToken());
			if (lexer.nextToken().type==comma)
			{
				lexer.nextToken();
				exp=new Expression(lexer);
			}
		}
		else
		{
			exp=new Expression(lexer);
		}
		if (lexer.currToken().type==rparen) lexer.nextToken();
		else warning(lexer,lost_rparen);
	}
	else
	{
		error(lexer,lost_lparen);
	}
}

Write::~Write()
{
	delete token;
	delete exp;
}

void Write::scan(Coder &coder,SymTab &symtab)
{
	if (exp) exp->scan(coder,symtab);
	coder.putTAC({semicolon});
}

void Write::genCode(Coder &coder,SymTab &symtab) const
{
	std::string fmt="'";
	Symbol symb;
	if (token) fmt+=token->s;
	if (exp)
	{
		exp->genCode(coder,symtab);
		symtab.find(exp->res.s,symb);
		fmt+=symb.type==int_t?"%hd":"%c";
	}
	fmt+="', 0Ah";
	coder.save_reg(symtab);
	coder.push_reg(symtab.getLevel());
	if (exp) coder.append({"movsx","rsi",symb.val()});
	coder.append({"mov","rdi","fmt"+to_string(coder.add(fmt))});
	coder.append({"xor","rax","rax"});
	coder.append({"call","printf"});
	coder.pop_reg(symtab.getLevel());
	coder.sync_reg(symtab);
}

Block::Block(Lexer &lexer)
{
	if (lexer.currToken().type==word_end)
	{
		lexer.nextToken();
	}
	else
	{
		statements.push_back(Statement::bear(lexer));
		while (lexer.currToken().type==semicolon)
		{
			lexer.nextToken();
			statements.push_back(Statement::bear(lexer));
		}
		if (lexer.currToken().type==word_end) lexer.nextToken();
		else error(lexer,lost_end);
	}
}

Block::~Block()
{
	for (const auto &i:statements) delete i;
}

void Block::scan(Coder &coder,SymTab &symtab)
{
	for (const auto &i:statements) if (i) i->scan(coder,symtab);
}

void Block::genCode(Coder &coder,SymTab &symtab) const
{
	for (const auto &i:statements) if (i) i->genCode(coder,symtab);
}

Program::Program(Lexer &lexer): block(nullptr)
{
	if (lexer.currToken().type==word_const)
	{
		do
		{
			lexer.nextToken();
			const_defs.push_back(new ConstDef(lexer));
		}
		while (lexer.currToken().type==comma);
		if (lexer.currToken().type==semicolon) lexer.nextToken();
		else error(lexer,lost_semicolon);
	}
	if (lexer.currToken().type==word_var)
	{
		lexer.nextToken();
		do
		{
			var_defs.push_back(new VarDef(lexer));
		}
		while (lexer.currToken().type==ident);
	}
	for (;;)
	{
		if (lexer.currToken().type==word_proc)
		{
			lexer.nextToken();
			proc_defs.push_back(new ProcDef(lexer));
		}
		else if (lexer.currToken().type==word_func)
		{
			lexer.nextToken();
			func_defs.push_back(new FuncDef(lexer));
		}
		else
		{
			break;
		}
	}
	if (lexer.currToken().type==word_begin)
	{
		lexer.nextToken();
		block=new Block(lexer);
	}
	else
	{
		error(lexer,lost_begin);
	}
}

Program::~Program()
{
	for (const auto &i:const_defs) delete i;
	for (const auto &i:var_defs) delete i;
	for (const auto &i:proc_defs) delete i;
	for (const auto &i:func_defs) delete i;
	delete block;
}

void Program::scan(Coder &coder,SymTab &symtab) const
{
	for (const auto &i:const_defs) i->scan(coder,symtab);
	for (const auto &i:var_defs) i->scan(coder,symtab);
	for (const auto &i:proc_defs) i->scan(coder,symtab);
	for (const auto &i:func_defs) i->scan(coder,symtab);
	block->scan(coder,symtab);
}

void Program::genCode(Coder &coder,SymTab &symtab,const std::string &val) const
{
	coder.append({"enter",to_string(symtab.localSize()),"0"});
	coder.append({"xchg","rbp",base_reg[symtab.getLevel()]});
	block->genCode(coder,symtab);
	if (!val.empty()) coder.append({"mov","ax",val});
	coder.append({"xchg","rbp",base_reg[symtab.getLevel()]});
	coder.append({"leave"});
	coder.append({"ret"});
	for (const auto &i:proc_defs) i->genCode(coder,symtab);
	for (const auto &i:func_defs) i->genCode(coder,symtab);
}
