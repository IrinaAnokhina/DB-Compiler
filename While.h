#ifndef WHILE_H
#define WHILE_H

#include"Stmt.h"
//S -> while (B) S1

class While : public Stmt
{
public:
	While(shared_ptr<Expr>x, shared_ptr<Stmt>s);

	void init(shared_ptr<Expr>, shared_ptr<Stmt>);
	virtual void gen(int, int) override;

	shared_ptr<Expr> expr;
	shared_ptr<Stmt> stmt;
};

While::While(shared_ptr<Expr>x, shared_ptr<Stmt>s)
	:expr(x), stmt(s) 
{}

void While::init(shared_ptr<Expr>x, shared_ptr<Stmt>s)
{
	expr = x;
	stmt = s;
	if (expr->type != Type::Bool)
		expr->error("bool required in if");
}

void While::gen(int b, int a)
{
	begin = b;
	//S.next
	after = a;
	//B.true = fall = 0      B.false = S.next 
	expr->jumping(0, a);
	//start of S1
	int label = newLabel();
	emitLabel(label);
	// S1.next = begin
	stmt->gen(label, b);
	emit("goto L" + std::to_string(b));
}

#endif