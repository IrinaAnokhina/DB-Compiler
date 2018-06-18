#ifndef ARITH_H
#define ARITH_H

#include"Op.h"
class Type;

class Arith : public Op
{
public:
	Arith(shared_ptr<Token>, shared_ptr<Expr>, shared_ptr<Expr>);

	virtual shared_ptr<Expr>gen() override;
	virtual string toString();

	shared_ptr<Expr> expr1;
	shared_ptr<Expr> expr2;
};

Arith::Arith(shared_ptr<Token>tok, shared_ptr<Expr>x1, shared_ptr<Expr>x2)
	: Op(tok, nullptr), expr1(x1), expr2(x2)
{
	type = type->maximum(expr1->type, expr2->type);
	if (type == nullptr)
		error("error type");
}

shared_ptr<Expr> Arith::gen()
{
	return std::make_shared<Arith>(op, expr1->reduce(), expr2->reduce());
}

string Arith::toString()
{
	return expr1->toString() + " " + op->toString() + " " + expr2->toString();
}

#endif

