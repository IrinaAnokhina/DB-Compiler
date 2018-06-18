#ifndef OR_H
#define OR_H

#include"Logical.h"

class Or : public Logical
{
public:
	Or(shared_ptr<Token>, shared_ptr<Expr>, shared_ptr<Expr>);

	virtual void jumping(int, int) override;
};

Or::Or(shared_ptr<Token> tok, shared_ptr<Expr> x1, shared_ptr<Expr> x2)
	: Logical(tok, x1, x2){}

void Or::jumping(int t, int f)
{
	int label = (t != 0 ? t : newLabel());
	expr1->jumping(label, 0);
	expr2->jumping(t, f);
	if (t == 0)
		emitLabel(label);
}

#endif