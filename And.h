#ifndef AND_H
#define AND_H

#include"Logical.h"

class And : public Logical
{
public:
	And(shared_ptr<Token>, shared_ptr<Expr>, shared_ptr<Expr>);

	virtual void jumping(int, int) override;
};

And::And(shared_ptr<Token> tok, shared_ptr<Expr> x1, shared_ptr<Expr> x2)
	: Logical(tok, x1, x2){}

void And::jumping(int t, int f)
{
	int label = (f != 0 ? f : newLabel());
	expr1->jumping(0, label);
	expr2->jumping(t, f);
	if (f == 0)
		emitLabel(label);
}
#endif