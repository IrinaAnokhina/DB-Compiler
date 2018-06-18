#ifndef ELSE_H
#define ELSE_H

#include"Stmt.h"
class Break;
class Continue;

class Else : public Stmt
{
public:
	// S -> if(B) S1 else S2
	Else(shared_ptr<Expr>, shared_ptr<Stmt>, shared_ptr<Stmt>);

	virtual void gen(int, int) override;

	shared_ptr<Expr>expr;
	shared_ptr<Stmt>stmt1;
	shared_ptr<Stmt> stmt2;
};

Else::Else(shared_ptr<Expr>x, shared_ptr<Stmt>s1, shared_ptr<Stmt>s2)
	: expr(x), stmt1(s1), stmt2(s2)
{
	if (expr->type != Type::Bool)
		expr->error("bool required in else");
}

void Else::gen(int b, int a)
{
	// label for begin S1
	int label1 = newLabel();
	// label for begin S2  
	int label2 = newLabel();
	//B.true = fall = 0     B.false = L2
	expr->jumping(0, label2);
	emitLabel(label1);
	//S1.next = S.next
	stmt1->gen(label1, a);
	// to prevent duplication goto statements
	if (!((typeid(Break) == typeid(*stmt1)) 
		|| (typeid(Continue) == typeid(*stmt1))))
	emit("goto L" + std::to_string(a));

	emitLabel(label2);
	//S2.next = S.next 
	stmt2->gen(label2, a);
}

#endif


