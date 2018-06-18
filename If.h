#ifndef IF_H
#define IF_H

#include"Stmt.h"
// S -> if (B) S1
class If : public Stmt
{
public:
	If(shared_ptr<Expr>, shared_ptr<Stmt>);

	virtual void gen(int, int) override;

	shared_ptr<Expr> expr;
	shared_ptr<Stmt> stmt;
};

If::If(shared_ptr<Expr>x, shared_ptr<Stmt>s)
	: expr(x), stmt(s)
{
	if (expr->type != Type::Bool)
		expr->error("bool required in if");
}

void If::gen(int b, int a)
{
	int label = newLabel();
	// label for the code for stmt
	expr->jumping(0, a);
	//B.true = fall(0)  B.false = after 
	emitLabel(label);
	// Label: S
	stmt->gen(label, a);
	// after: S.next
}

#endif