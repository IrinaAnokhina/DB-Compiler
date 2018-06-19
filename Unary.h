#ifndef UNARY_H
#define UNARY_H

#include"Op.h"

class Unary : public Op
{
public:
	Unary(shared_ptr<Token>, shared_ptr<Expr>);

	virtual shared_ptr<Expr> gen() override;
	virtual string toString() override;

	shared_ptr<Expr> exp;
};

Unary::Unary(shared_ptr<Token>tok, shared_ptr<Expr>x)
	: Op(tok, nullptr), exp(x)
{
	type = type->maximum(Type::Int, exp->type);
	if (type == nullptr)
		error("error type");
}

shared_ptr<Expr> Unary::gen()
{
	return std::make_shared<Unary>(op, exp->reduce());
}

string Unary::toString()
{
	return op->toString() + " " + exp->toString();
}

#endif