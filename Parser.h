
#pragma warning(disable : 4996)
#ifndef PARSER_H
#define PARSER_H

#include<iostream>
using std::cout;
#include<memory>
using std::dynamic_pointer_cast;
#include<windows.h>
#include"Lexer.h"

#include"Stmt.h"
#include"Env.h"
#include"If.h"
#include"Else.h"
#include"Seq.h"
#include"Array.h"
#include"While.h"
#include"Do.h"
#include"Break.h"
#include"Set.h"
#include"Rel.h"
#include"Unary.h"
#include"Not.h"
#include"Constant.h"
#include"Access.h"
#include"SetElem.h"
#include"Or.h"
#include"And.h"
#include"Arith.h"
#include"Continue.h"
#include"For.h"
class Id;
class Word;


class Parser
{
public:
	Parser(shared_ptr<Lexer>, HWND);
	~Parser();
	void move()  { look = lex->scan(); }
	void error(string s); 
	//check syntax match
	void match(int);
	void program();
	shared_ptr<Stmt>block();
	void decls();
	shared_ptr<Type>type();
	shared_ptr<Type>dims(shared_ptr<Type>);
	shared_ptr<Stmt>stmts();
	shared_ptr<Stmt>stmt();
	shared_ptr<Stmt>assign();
	shared_ptr<Expr>boolen();
	shared_ptr<Expr>join();
	shared_ptr<Expr>equality();
	shared_ptr<Expr>rel();
	shared_ptr<Expr>expr();
	shared_ptr<Expr>term();
	shared_ptr<Expr>unary();
	shared_ptr<Expr>factor();
	shared_ptr<Access>offset(shared_ptr<Id>);

private:
	//lexical analyzer
	shared_ptr<Lexer> lex;
	//lookahead
	shared_ptr<Token> look;
	//current symbol table 
	shared_ptr<Env>top;
	// memory offset for declarations
	int used = 0;
	HWND hwnd;
};

Parser::Parser(shared_ptr<Lexer>l, HWND h)
	: lex(std::move(l)), hwnd(h)
{
	move();
}
Parser::~Parser()
{
	Node::labels = 0;              //  don`t work!!
}
void Parser::match(int t)
{
	if (look->tag == t)
		move();
	else
		error("syntax error in match " + std::to_string(look->tag) + " != " + std::to_string(t));
}
// program -> block()
void Parser::program()
{
	shared_ptr<Stmt> s = block();
	int begin = s->newLabel();
	int after = s->newLabel();
	s->emitLabel(begin);
	s->gen(begin, after);
	s->emitLabel(after);
}

void Parser::error(string s)
{
	string message = "near line " + std::to_string(lex->line) + ": ";
	s = message + s;
	MessageBox(hwnd, s.c_str(), "untitled", MB_OK | MB_ICONERROR);
	exit(0);
}
// block -> {decls stmts}
shared_ptr<Stmt> Parser::block()
{
	match('{');
	shared_ptr<Env>savedEnv = top;
	// create a current unit of symbol table for this block
	top = std::make_shared<Env>(top);
	 decls();
	shared_ptr<Stmt>s = stmts();
	match('}');
	top = savedEnv;
	return s;
}
// decls -> decls decl | null
void Parser::decls()
{
	//decl -> type id
	while (look->tag == Tag::BASIC)
	{
		shared_ptr<Type>p = type();
		shared_ptr<Token>tok = look;
		match(Tag::ID);
		match(';');
		shared_ptr<Word>w = dynamic_pointer_cast<Word>(tok);
		if (w != 0)
		{
			shared_ptr<Id> id = std::make_shared<Id>(w, p, used);
			top->put(tok, id);
			used = (used + p->width);
		}
		else
			error("dynamic_cast error");	
	}
}

shared_ptr<Type> Parser::type()
{
	shared_ptr<Type>p = std::dynamic_pointer_cast<Type>(look);
	// type -> basic 
	match(Tag::BASIC);
	if (look->tag != '[')
		return p;
	else
		return dims(p);
	// type -> type [name]
}

shared_ptr<Type> Parser::dims(shared_ptr<Type>p)
{
	match('[');
	shared_ptr<Token>tok = look;
	// integer type of array index
	match(Tag::NUM);
	match(']');
	if (look->tag == '[')
		p = dims(p);
	shared_ptr<Num> n = dynamic_pointer_cast<Num>(tok);
	if (n != 0)
		return  std::make_shared<Array>(n->value, p);
	else
		error("dynamic_cast error");
}

shared_ptr<Stmt> Parser::stmts()
	{
	// stmts -> null
		if (look->tag == '}')
		{
			return Stmt::Null;
		}
		//stmts -> stmt stmts  
			else
			{
				shared_ptr<Stmt>one = stmt();
				shared_ptr<Stmt>two = stmts();
				return std::make_shared<Seq>(one, two);
			}
	}

	shared_ptr<Stmt> Parser::stmt()
	{
		shared_ptr<Stmt> s1;
		shared_ptr<Stmt> s2;
		shared_ptr<Stmt> s3;
		// enclosing structure for Break- and continue-statement
		shared_ptr<Stmt> savedStmt;
	shared_ptr<Expr> x;
	shared_ptr<While> whilenode;
	shared_ptr<Do> donode;
	shared_ptr<For>fornode;
	switch (look->tag)
	{
	case ';':
		move();
		return Stmt::Null;
		// stmt -> if (bool) stmt
	case Tag::IF:
		match(Tag::IF);
		match('(');
		x = boolen();
		match(')');
		s1 = stmt();
		if (look->tag != Tag::ELSE)
			return std::make_shared<If>(x, s1);
		// stmt -> if (bool) stmt else stmt  
		match(Tag::ELSE);
		s2 = stmt();
		return std::make_shared<Else>(x, s1, s2);
	case Tag::WHILE:
		// stmt -> while (bool) stmt
		whilenode = std::make_shared<While>(x, s1);
		savedStmt = Stmt::Enclosing;
		Stmt::Enclosing = whilenode;
		match(Tag::WHILE);
		match('(');
		x = boolen();
		match(')');
		s1 = stmt();
		whilenode->init(x, s1);
		Stmt::Enclosing = savedStmt;
		return whilenode;
	case Tag::DO:
		// stmt -> do stmt while (bool);  
		donode = std::make_shared<Do>(s1, x);
		savedStmt = Stmt::Enclosing;
		Stmt::Enclosing = donode;
		match(Tag::DO);
		s1 = stmt();
		match(Tag::WHILE);
		match('(');
		x = boolen();
		match(')');
		match(';');
		donode->init(s1, x);
		Stmt::Enclosing = savedStmt;
		return donode;
	case Tag::FOR:     
		// stmt-> for(stmt; bool; stmt;) stmt
		fornode = std::make_shared<For>(s1, x, s2, s3);
		savedStmt = Stmt::Enclosing;
		Stmt::Enclosing = fornode;
		match(Tag::FOR);
		match('(');
		s1 = stmt();
		x = boolen();
		match(';');
		s2 = stmt();
		match(')');
		s3 = stmt();
		fornode->init(s1, x, s2, s3);
		Stmt::Enclosing = savedStmt;
		return fornode;
	case Tag::BREAK:
		// stmt -> break;
		match(Tag::BREAK);
		match(';');
		return std::make_shared<Break>();  
	case Tag::CONTINUE:								
		// stmt -> continue;
		match(Tag::CONTINUE);
		match(';');
		return std::make_shared<Continue>();
	case '{':
		// stmt -> block
		return block();
	default:
		return assign();
		// stmt -> loc = bool
	}
}

shared_ptr<Stmt> Parser::assign()
{
	shared_ptr<Stmt>stmt;
	shared_ptr<Token> t = look;

	match(Tag::ID);
	shared_ptr<Id>id = (top->get(t));
	if (id == NULL)
		error(t->toString() + " undeclared");
	if (look->tag == '=')
	{
		move();
		// loc -> id = E
		stmt = std::make_shared<Set>(id, boolen());
	}
	else
	{
		// loc -> loc [boolen] = E
		shared_ptr<Access>ac = offset(id);
		match('=');
		stmt = std::make_shared<SetElem>(ac, boolen());
	}
	match(';');
	return stmt;
}

shared_ptr<Expr> Parser::boolen()
{
	// boolen -> join 
	shared_ptr<Expr>x = join();
	// boolen -> bool || join
	while (look->tag == Tag::OR)
	{
		shared_ptr<Token>tok = look;
		move();
		x = std::make_shared<Or>(tok, x, join());
	}
	return x;
}

shared_ptr<Expr> Parser::join()
{
	// join -> equality 
	shared_ptr<Expr> x = equality();
	// join -> join && equality
	while (look->tag == Tag::AND)
	{
		shared_ptr<Token>tok = look;
		move();
		x = std::make_shared<And>(tok, x, equality());
	}
	return x;
}

shared_ptr<Expr> Parser::equality()
{
	// equality -> rel
	shared_ptr<Expr>x = rel();
	// equality -> equality == rel
	// equality -> equality != rel 
	while (look->tag == Tag::EQ || look->tag == Tag::NE)
	{
		shared_ptr<Token>tok = look;
		move();
		x = std::make_shared<Rel>(tok, x, rel());
	}
	return x;
}

shared_ptr<Expr> Parser::rel()
{
	shared_ptr<Expr> x = expr();
	shared_ptr<Token>tok = look;
	shared_ptr<Expr>e;
	switch (look->tag)
	{
		// bool -> expr < expr
		// bool -> expr <= expr 
	case '<':
	case Tag::LE:
	case Tag::GE:
		// bool -> expr >= expr 
		// bool -> expr > expr  
	case '>':
		move();
		e = expr();
		return std::make_shared<Rel>(tok, x, e);
	default:
		// rel -> expr
		return x;
	}
}

shared_ptr<Expr> Parser::expr()
{
	// expr -> term 
	shared_ptr<Expr> x = term();
	// expr -> expr + term 
	// expr -> expr - term
	while (look->tag == '+' || look->tag == '-')
	{
		shared_ptr<Token> tok = look;
		move();
		x = std::make_shared<Arith>(tok, x, term());
	}
	return x;
}

shared_ptr<Expr>Parser::term()
{
	// term -> unary
	shared_ptr<Expr> x = unary();
	// term -> term * unary 
	// term -> term / unary
	while (look->tag == '*' || look->tag == '/')
	{
		shared_ptr<Token>tok = look;
		move();
		x = std::make_shared<Arith>(tok, x, unary());
	}
	return x;
}

shared_ptr<Expr>Parser::unary()
{
	if (look->tag == '-')
	{
		move();
		// unary -> -unary
		return std::make_shared<Unary>(Word::minus, unary());
	}
	else if (look->tag == '!')
	{
		// unary -> !unary   
		shared_ptr<Token>tok = look;
		move();
		return std::make_shared<Not>(tok, unary());
	}
	else
		return factor();
	// unary -> factor
}

shared_ptr<Expr> Parser::factor()
{
	shared_ptr<Expr>x;
	switch (look->tag)
	{
		// factor -> (E)
	case '(':
		move();
		x = boolen();
		match(')');
		return x;
	case Tag::NUM:
		// factor -> num 
		x = std::make_shared<Constant>(look, Type::Int);
		move(); return x;
	case Tag::REAL:
		// factor -> real
		x = std::make_shared<Constant>(look, Type::Float);
		move(); return x;
	case Tag::TRue:
		// factor -> true
		x = Constant::TRue;
		move(); return x;
	case Tag::FAlse:
		// factor -> false
		x = Constant::FAlse;
		move(); return x;
	default:
		error("syntax error");
		return x;
	case Tag::ID:
		// factor -> loc
		string s = look->toString();
		shared_ptr<Id>id = top->get(look);
		if (id == nullptr)
			error(look->toString() + " undeclared");
		move();
		// loc -> id
		if (look->tag != '[')
			return id;
		else
			return offset(id);
		// loc -> loc[E]  || loc[E] I
	}
}

shared_ptr<Access> Parser::offset(shared_ptr<Id>a)
{
	shared_ptr<Expr>i;
	shared_ptr<Expr> w;
	shared_ptr<Expr>t1;
	shared_ptr<Expr>t2;
	shared_ptr<Expr>loc;
	shared_ptr<Type>type = a->type;
	// first index, I -> [E]
	match('[');
	i = boolen(); match(']');
	type = (dynamic_pointer_cast<Array>(type))->of;
	w = std::make_shared<Constant>(type->width);
	t1 = std::make_shared<Arith>(std::make_shared<Token>('*'), i, w);
	loc = t1;
	// multidimensional  I -> [E] I
	while (look->tag == '[')
	{
		match('[');
		i = boolen(); match(']');
		type = (dynamic_pointer_cast<Array>(type))->of;
		w = std::make_shared<Constant>(type->width);
		t1 = std::make_shared<Arith>(std::make_shared<Token>('*'), i, w);
		t2 = std::make_shared<Arith>(std::make_shared<Token>('+'), loc, t1);
		loc = t2;
	}
	return std::make_shared<Access>(a, loc, type);
}
#endif
