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

void ConstDef::genCode(Coder &coder,SymTab &symtab) const
{
	Symbol s;
	s.kind=constant;
	s.type=value->type==number?int_t:char_t;
	s.v=value->v;
	s.size=value->type==number?INT_SIZE:CHAR_SIZE;
	symtab.insert(name->s,s);
	//coder.append("sub","esp",std::to_string(INT_SIZE));
	//coder.append(Instr("mov","WORD PTR [esp+"+std::to_string(symtab.getSP()-s.addr)+"]",std::to_string(s.v));
}

VarDef::VarDef(Lexer &lexer,bool f): size(-1),type(nullptr)
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
			else if (f) warning(lexer,lost_semicolon);
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

void VarDef::genCode(Coder &coder,SymTab &symtab) const
{

}

ProcDef::ProcDef(Lexer &lexer): name(new Token(lexer.currToken())),program(nullptr)
{
	if (lexer.nextToken().type==lparen)
	{
		bool var;
		if ((var=lexer.nextToken().type==word_var))
			lexer.nextToken();
		para_list.push_back(make_pair(var,new VarDef(lexer,false)));
		while (lexer.currToken().type==semicolon)
		{
			if ((var=lexer.nextToken().type==word_var))
				lexer.nextToken();
			para_list.push_back(make_pair(var,new VarDef(lexer)));
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

void ProcDef::genCode(Coder &coder,SymTab &symtab) const
{

}

FuncDef::FuncDef(Lexer &lexer): name(new Token(lexer.currToken())),
		type(nullptr),program(nullptr)
{
	if (lexer.nextToken().type==lparen)
	{
		bool var;
		if ((var=lexer.nextToken().type==word_var))
			lexer.nextToken();
		para_list.push_back(make_pair(var,new VarDef(lexer,false)));
		while (lexer.currToken().type==semicolon)
		{
			if ((var=lexer.nextToken().type==word_var))
				lexer.nextToken();
			para_list.push_back(make_pair(var,new VarDef(lexer)));
		}
		if (lexer.currToken().type==rparen) lexer.nextToken();
		else warning(lexer,lost_rparen);
	}
	if (lexer.currToken().type==colon) lexer.nextToken();
	else error(lexer,lost_colon);
	if (lexer.currToken().type==word_integer ||
			lexer.currToken().type==word_char) lexer.nextToken();
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

void FuncDef::genCode(Coder &coder,SymTab &symtab) const
{

}

Factor::Factor(Lexer &lexer): token(nullptr),exp(nullptr)
{
	if (lexer.currToken().type==lparen)
	{
		lexer.nextToken();
		exp=new Expression(lexer);
	}
	else
	{
		token=new Token(lexer.currToken());
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
}

Factor::~Factor()
{
	delete token;
	delete exp;
	for (const auto &i:arg_list) delete i;
}

void Factor::genCode(Coder &coder,SymTab &symtab) const
{

}

Term::Term(Lexer &lexer)
{
	Token *token=nullptr;
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

void Term::genCode(Coder &coder,SymTab &symtab) const
{

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

void Expression::genCode(Coder &coder,SymTab &symtab) const
{

}

Condition::Condition(Lexer &lexer): token(nullptr),exp0(nullptr),exp1(nullptr)
{
	exp0=new Expression(lexer);
	const Token temp=lexer.currToken();
	if (eql<=temp.type && temp.type<=geq)
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

void Condition::genCode(Coder &coder,SymTab &symtab) const
{

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

void Assignment::genCode(Coder &coder,SymTab &symtab) const
{

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

void ProcCall::genCode(Coder &coder,SymTab &symtab) const
{

}

DoWhile::DoWhile(Lexer &lexer): condition(nullptr),statement(nullptr)
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

void DoWhile::genCode(Coder &coder,SymTab &symtab) const
{

}

ForDo::ForDo(Lexer &lexer): token0(nullptr),token1(nullptr),exp0(nullptr),exp1(nullptr),statement(nullptr)
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

void ForDo::genCode(Coder &coder,SymTab &symtab) const
{

}

IfThen::IfThen(Lexer &lexer): condition(nullptr),statement0(nullptr),statement1(nullptr)
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

void IfThen::genCode(Coder &coder,SymTab &symtab) const
{
	//condition->genCode();
	//coder.
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

void Read::genCode(Coder &coder,SymTab &symtab) const
{
	Symbol symb;
	for (auto &i:tokens)
	{
		if (symtab.find(i->s,symb))
		{
			if (symb.kind==variable)
			{
				std::string fmt;
				if (symb.type==int_t) fmt="%d";
				if (symb.type==char_t) fmt="%c";
				coder.append({"mov","rdi","fmt"+std::to_string(coder.add(fmt))});
				coder.append({"mov","rsi","rax"});
				coder.append({"xor","rax","rax"});
				coder.append({"call","scanf",""});
			}
			else
			{
				error(i->s,cannot_read);
			}
		}
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

void Write::genCode(Coder &coder,SymTab &symtab) const
{
	std::string fmt;
	if (token) fmt+=token->s;
	if (exp)
	{
		exp->genCode(coder,symtab);
		fmt+="%d";
	}
	coder.append({"mov","rdi","fmt"+std::to_string(coder.add(fmt))});
	coder.append({"mov","rsi","rax"});
	coder.append({"xor","rax","rax"});
	coder.append({"call","printf",""});
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
		default:break;
	}
	return nullptr;
}

Block::Block(Lexer &lexer)
{
	if (lexer.currToken().type!=word_end)
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

void Block::genCode(Coder &coder,SymTab &symtab) const
{
	for (const auto &i:statements) i->genCode(coder,symtab);
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

void Program::genCode(Coder &coder,SymTab &symtab) const
{
	for (const auto &i:const_defs) i->genCode(coder,symtab);
	for (const auto &i:var_defs) i->genCode(coder,symtab);
	for (const auto &i:proc_defs) i->genCode(coder,symtab);
	for (const auto &i:func_defs) i->genCode(coder,symtab);
	block->genCode(coder,symtab);
}
