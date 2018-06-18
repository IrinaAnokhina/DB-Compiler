#ifndef CONSTANT_H
#define CONSTANT_H

#include"Expr.h"

class Constant : public Expr
{
public:
	Constant(shared_ptr<Token>, shared_ptr<Type>);
	Constant(int);

	virtual void jumping(int, int) override;

	static shared_ptr<Constant> TRue;
	static shared_ptr<Constant> FAlse;
};

shared_ptr<Constant> Constant::TRue = std::make_shared<Constant>(Word::True, Type::Bool);
shared_ptr<Constant> Constant::FAlse = std::make_shared< Constant>(Word::False, Type::Bool);

Constant::Constant(shared_ptr<Token>tok, shared_ptr<Type>t)
	: Expr(tok, t){}

Constant::Constant(int i)
	: Expr(std::make_shared<Num>(i), Type::Int) {}

void Constant::jumping(int t, int f)
{
	shared_ptr<Constant>l = dynamic_pointer_cast<Constant>(getptr());
	if (l == TRue && t != 0)
		emit("goto L" + std::to_string(t));
	else if (l == FAlse && f != 0)          
		emit("goto L" + std::to_string(f));
}

#endif