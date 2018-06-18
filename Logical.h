#ifndef LOGIC_H
#define LOGIC_H

#include"Expr.h"
#include"Temp.h"

class Logical : public Expr
{
public:
	Logical(shared_ptr<Token>, shared_ptr<Expr>, shared_ptr<Expr>);

	virtual shared_ptr<Type> check(shared_ptr<Type>, shared_ptr<Type>);
	virtual shared_ptr<Expr>gen() override;
	virtual string toString() override;
	shared_ptr<Expr> expr1;
	shared_ptr<Expr> expr2;
};

Logical::Logical(shared_ptr<Token> tok, shared_ptr<Expr> x1, shared_ptr<Expr> x2)
	: Expr(tok,nullptr), expr1(x1), expr2(x2)
{
	type = check(expr1->type, expr2->type);
	if (type == nullptr)
		error("type error in logical");
}

shared_ptr<Type> Logical::check(shared_ptr<Type> t1, shared_ptr<Type> t2)
{
	if (t1 == Type::Bool && t2 == Type::Bool)
		return Type::Bool;
	if (t1 == Type::Int && t2 == Type::Int)
		return Type::Bool;
	if (t1 == Type::Float && t2 == Type::Float)
		return Type::Bool;
	if (t1 == Type::Char && t2 == Type::Char)
		return Type::Bool;
	else
		return nullptr;
}

shared_ptr<Expr> Logical::gen()
{
	int f = newLabel();
	int a = newLabel();
	shared_ptr<Temp>temp = std::make_shared<Temp>(type);
	shared_ptr<Logical>l = dynamic_pointer_cast<Logical>(getptr());                     
	if (l != 0)
		l->jumping(0, f);
	else
		error("dynamic_cast error in Logical");
		                               
	emit(temp->toString() + " = true");
	emit("goto L" + std::to_string(a));
	emitLabel(f);
	emit(temp->toString() + " = false");
	emitLabel(a);
	return temp;
}

string Logical::toString()
{
	return expr1->toString() + " " + op->toString() + " " + expr2->toString();
}

#endif