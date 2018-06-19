#ifndef SET_H
#define SET_H

#include"Stmt.h"
#include"Id.h"
//Id = Expr

class Set : public Stmt
{
public:
	Set(shared_ptr<Id>, shared_ptr<Expr>);

	shared_ptr<Type>check(shared_ptr<Type>, shared_ptr<Type>);
	virtual void gen(int, int) override;

	shared_ptr<Id>id;
	shared_ptr<Expr>expr;
};

Set::Set(shared_ptr<Id>i, shared_ptr<Expr>x)
	: id(i), expr(x)
{
	if (check(id->type, expr->type) == nullptr)
		error("type error");
}

shared_ptr<Type> Set::check(shared_ptr<Type> t1, shared_ptr<Type> t2)
{
	if (Type::numeric(t1) && Type::numeric(t2))
		return t1;                                    
	else if (t1 == Type::Bool && t2 == Type::Bool)
		return Type::Bool;
	else
		return nullptr;
	}

void Set::gen(int b, int a)
{
	emit(id->toString() + " = " + expr->gen()->toString());
}

#endif