#ifndef DO_H
#define DO_H

#include"Stmt.h"

class Do : public Stmt
{
public:
	// S -> do S1 while(B)
	Do(shared_ptr<Stmt>s, shared_ptr<Expr>x);

	void init(shared_ptr<Stmt>, shared_ptr<Expr>);
	virtual void gen(int, int) override;

	
	shared_ptr<Stmt> stmt;
	shared_ptr<Expr> expr;
};

Do::Do(shared_ptr<Stmt>s, shared_ptr<Expr>x)
	: stmt(s), expr(x)
{}

void Do::init(shared_ptr<Stmt>s, shared_ptr<Expr>x)
{
	stmt = s;
	expr = x;
	if (expr->type != Type::Bool)
		expr->error("bool required in do");
}

void Do::gen(int b, int a)
{
	//S.next
	after = a; 
	// start of B 
	int label = newLabel();
	//start of S1 = b       S1.next = label
	stmt->gen(b, label);
	emitLabel(label);
	//B.true = b     B.false = fall = 0
	expr->jumping(b, 0);
}

#endif