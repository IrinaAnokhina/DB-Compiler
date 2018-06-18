#ifndef REL_H
#define REL_H

#include"Logical.h"
#include"Array.h"

class Rel : public Logical
{
public:
	Rel(shared_ptr<Token>, shared_ptr<Expr>, shared_ptr<Expr>);

	virtual shared_ptr<Type> check(shared_ptr<Type>, shared_ptr<Type>) override;
	virtual void jumping(int, int) override;
};

Rel::Rel(shared_ptr<Token> tok, shared_ptr<Expr>x1, shared_ptr<Expr> x2)
	: Logical(tok, x1, x2){}

shared_ptr<Type> Rel::check(shared_ptr<Type> t1, shared_ptr<Type>t2)
{
	shared_ptr<Array> ar1 = dynamic_pointer_cast<Array>(t1);
	shared_ptr<Array>ar2 = dynamic_pointer_cast<Array>(t2);
	if (ar1 != 0 || ar2 != 0)
		return nullptr;
	else if (t1 == t2)
		return Type::Bool;
	else
		return nullptr;
}
void Rel::jumping(int t, int f)
{
	shared_ptr<Expr> a = expr1->reduce();
	shared_ptr<Expr> b = expr2->reduce();

	string test = a->toString() + " " + op->toString() + " " + b->toString();
	emitJumps(test, t, f);
}
#endif