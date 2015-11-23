/*
 * parser.cxx
 *
 * Copyright 2015 constroy <constroy.li@gmail.com>
 *
 *
 */


#include "parser.hxx"

ConstDef::ConstDef(Lexer &lexer): name(0),value(0)
{
	if (lexer.currToken().type==ident)
	{
		name=new Token(lexer.currToken());
		if (lexer.nextToken().type!=eql) warning(lexer.getPos(),lost_eql);
		if (lexer.nextToken().type==number && lexer.nextToken().type==character)
		{
			value=new Token(lexer.currToken());
			lexer.nextToken();
		}
		else
		{
			error(lexer.getPos(),lost_num_or_char);
		}
	}
	else
	{
		error(lexer.getPos(),lost_ident);
	}
}
VarDef::VarDef(Lexer &lexer): size(-1),type(0)
{
	do
	{
		names.push_back(new Token(lexer.currToken()));
	}
	while (lexer.nextToken().type==comma);
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
					else warning(lexer.getPos(),lost_rbracket);
					if (lexer.currToken().type==word_of) lexer.nextToken();
					else warning(lexer.getPos(),lost_of);
				}
				else
				{
					error(lexer.getPos(),lost_number);
				}
			}
			else
			{
				error(lexer.getPos(),lost_lbracket);
			}
		}
		if (lexer.currToken().type==word_integer ||
				lexer.currToken().type==word_char)
		{
			type=new Token(lexer.currToken());
			if (lexer.nextToken().type==semicolon) lexer.nextToken();
			else warning(lexer.getPos(),lost_semicolon);
		}
		else
		{
			error(lexer.getPos(),lost_type);
		}
	}
	else
	{
		error(lexer.getPos(),lost_colon);
	}
}
ProcDef::ProcDef(Lexer &lexer)
{
	name=new Token(lexer.currToken());
	if (lexer.currToken().type==lparen && lexer.nextToken().type!=rparen)
	{
		bool var;
		if ((var=lexer.currToken().type==word_var))
			lexer.nextToken();
		para_list.push_back(make_pair(var,new VarDef(lexer)));
		while (lexer.currToken().type==semicolon)
		{
			if ((var=lexer.nextToken().type==word_var))
				lexer.nextToken();
			para_list.push_back(make_pair(var,new VarDef(lexer)));
		}
		if (lexer.currToken().type==rparen) lexer.nextToken();
		else warning(lexer.getPos(),lost_rparen);
	}
	if (lexer.currToken().type==semicolon) lexer.nextToken();
	else warning(lexer.getPos(),lost_semicolon);
	program=new Program(lexer);
	if (lexer.currToken().type==semicolon) lexer.nextToken();
	else warning(lexer.getPos(),lost_semicolon);
}
FuncDef::FuncDef(Lexer &lexer)
{
	name=new Token(lexer.currToken());
	if (lexer.currToken().type==lparen && lexer.nextToken().type!=rparen)
	{
		bool var;
		if ((var=lexer.currToken().type==word_var))
			lexer.nextToken();
		para_list.push_back(make_pair(var,new VarDef(lexer)));
		while (lexer.currToken().type==semicolon)
		{
			if ((var=lexer.nextToken().type==word_var))
				lexer.nextToken();
			para_list.push_back(make_pair(var,new VarDef(lexer)));
		}
		if (lexer.currToken().type==rparen) lexer.nextToken();
		else warning(lexer.getPos(),lost_rparen);
	}
	if (lexer.currToken().type==colon) lexer.nextToken();
	else error(lexer.getPos(),lost_colon);
	if (lexer.currToken().type==word_integer ||
			lexer.currToken().type==word_char) lexer.nextToken();
	else error(lexer.getPos(),lost_type);
	if (lexer.currToken().type==semicolon) lexer.nextToken();
	else warning(lexer.getPos(),lost_semicolon);
	program=new Program(lexer);
	if (lexer.currToken().type==semicolon) lexer.nextToken();
	else warning(lexer.getPos(),lost_semicolon);
}
ProcCall::ProcCall(const Token &token,Lexer &lexer): name(new Token(token))
{
	if (lexer.currToken().type==lparen && lexer.nextToken().type!=rparen)
	{
		para_list.push_back(new Expression(lexer));
		while (lexer.currToken().type==comma)
		{
			lexer.nextToken();
			para_list.push_back(new Expression(lexer));
		}
		if (lexer.currToken().type==rparen) lexer.nextToken();
		else warning(lexer.getPos(),lost_rparen);
	}
}
Factor::Factor(Lexer &lexer): token(0),exp(0)
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
			else warning(lexer.getPos(),lost_rbracket);
		}
		else if (lexer.currToken().type==lparen &&
				lexer.nextToken().type!=rparen)
		{
			para_list.push_back(new Expression(lexer));
			while (lexer.currToken().type==comma)
			{
				lexer.nextToken();
				para_list.push_back(new Expression(lexer));
			}
			if (lexer.currToken().type==rparen) lexer.nextToken();
			else warning(lexer.getPos(),lost_rparen);
		}
	}
}
Term::Term(Lexer &lexer)
{
	Token *token=0;
	do
	{
		factors.push_back(make_pair(token,new Factor(lexer)));
		token=new Token(lexer.currToken());
		lexer.nextToken();
	}
	while (token->type==plus || token->type==minus);
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
	do
	{
		terms.push_back(make_pair(token,new Term(lexer)));
		token=new Token(lexer.currToken());
		lexer.nextToken();
	}
	while (token->type==plus || token->type==minus);
}
Condition::Condition(Lexer &lexer): token(0),exp0(0),exp1(0)
{
	exp0=new Expression(lexer);
	const Token &temp=lexer.currToken();
	if (eql<=temp.type && temp.type<=geq)
	{
		token=new Token(temp);
		lexer.nextToken();
		exp1=new Expression(lexer);
	}
	else
	{
		error(lexer.getPos(),lost_relational);
	}
}
Assignment::Assignment(const Token &token,Expression *exp,Lexer &lexer):
		dest(new Token(token)),
		exp0(exp),
		exp1(new Expression(lexer)) {}
DoWhile::DoWhile(Lexer &lexer)
{
	statement=new Statement(lexer);
	if (lexer.currToken().type==word_while)
	{
		lexer.nextToken();
		condition=new Condition(lexer);
	}
	else
	{
		error(lexer.getPos(),lost_while);
	}
}
ForDo::ForDo(Lexer &lexer)
{
	token0=lexer.currToken();
	if (lexer.nextToken().type==assign)
	{
		lexer.nextToken();
		exp0=new Expression(lexer);
		if (lexer.currToken().type==word_to ||
			lexer.currToken().type==word_downto)
		{
			exp1=new Expression(lexer);
			if (lexer.currToken().type==word_do)
			{
				lexer.nextToken();
				statement=new Statement(lexer);
			}
			else
			{
				error(lexer.getPos(),lost_do);
			}
		}
		else
		{
			error(lexer.getPos(),lost_to);
		}
	}
	else
	{
		error(lexer.getPos(),lost_assign);
	}
}
IfThen::IfThen(Lexer &lexer)
{
	condition=new Condition(lexer);
	if (lexer.currToken().type==word_then)
	{
		lexer.nextToken();
		statement0=new Statement(lexer);
		if (lexer.currToken().type==word_else)
			statement1=new Statement(lexer);
	}
	else
	{
		error(lexer.getPos(),lost_then);
	}
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
		else warning(lexer.getPos(),lost_rparen);
	}
	else
	{
		error(lexer.getPos(),lost_lparen);
	}
}
Write::Write(Lexer &lexer): token(0),exp(0)
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
			if (lexer.currToken().type==rparen) lexer.nextToken();
			else warning(lexer.getPos(),lost_rparen);
		}
		else
		{
			exp=new Expression(lexer);
			if (lexer.currToken().type==rparen) lexer.nextToken();
			else warning(lexer.getPos(),lost_rparen);
		}
	}
	else
	{
		error(lexer.getPos(),lost_lparen);
	}
}
Statement::Statement(Lexer &lexer): assignment(0),proc_call(0),
		do_while(0),for_do(0),if_then(0),read(0),write(0),block(0)
{
	const Token &token=lexer.currToken();
	switch (token.type)
	{
		case ident:
		{
			if (lexer.nextToken().type==lbracket)
			{
				lexer.nextToken();
				Expression *exp=new Expression(lexer);
				if (lexer.currToken().type==rbracket) lexer.nextToken();
				else warning(lexer.getPos(),lost_rbracket);
				if (lexer.currToken().type==assign)
					assignment=new Assignment(token,exp,lexer);
			}
			else if (lexer.currToken().type==assign)
			{
				lexer.nextToken();
				assignment=new Assignment(token,0,lexer);
			}
			else
			{
				proc_call=new ProcCall(token,lexer);
			}
			break;
		}
		case word_do:
		{
			lexer.nextToken();
			do_while=new DoWhile(lexer);
			break;
		}
		case word_for:
		{
			lexer.nextToken();
			for_do=new ForDo(lexer);
			break;
		}
		case word_if:
		{
			lexer.nextToken();
			if_then=new IfThen(lexer);
			break;
		}
		case word_read:
		{
			lexer.nextToken();
			read=new Read(lexer);
			break;
		}
		case word_write:
		{
			lexer.nextToken();
			write=new Write(lexer);
			break;
		}
		case word_begin:
		{
			lexer.nextToken();
			block=new Block(lexer);
			break;
		}
		default:break;
	}
}
Block::Block(Lexer &lexer)
{
	if (lexer.nextToken().type!=word_end)
	{
		statements.push_back(new Statement(lexer));
		while (lexer.currToken().type==semicolon)
		{
			lexer.nextToken();
			statements.push_back(new Statement(lexer));
		}
		if (lexer.currToken().type==word_end) lexer.nextToken();
		else error(lexer.getPos(),lost_end);
	}
}
Program::Program(Lexer &lexer): block(0)
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
		else error(lexer.getPos(),lost_semicolon);
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
	if (lexer.currToken().type==word_begin) block=new Block(lexer);
	else error(lexer.getPos(),lost_begin);
}
Parser::Parser(char file[]): program(0),lexer(file)
{
	lexer.nextToken();
}
Program *Parser::genAST()
{
	program=new Program(lexer);
	if (lexer.currToken().type!=period)
		warning(lexer.getPos(),lost_period);
	return program;
}
