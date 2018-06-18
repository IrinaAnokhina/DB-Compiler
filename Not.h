#ifndef NOT_H
#define NOT_H

#include"Logical.h"

class Not : public Logical
{
public:
	Not(shared_ptr<Token>, shared_ptr<Expr>);

	virtual void jumping(int, int) override;
	virtual string toString() override;
};

Not::Not(shared_ptr<Token> tok, shared_ptr<Expr> x2)
	: Logical(tok, x2, x2){}

void Not::jumping(int t, int f)
{
	expr2->jumping(f, t);
}

string Not::toString()
{
	return op->toString() + " " + expr2->toString();
}
#endif