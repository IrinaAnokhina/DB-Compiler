#ifndef OP_H
#define OP_H

#include"Expr.h"
#include"Temp.h"
class Type;

class Op : public Expr
{
public:
	Op(shared_ptr<Token>, shared_ptr<Type>);

	virtual shared_ptr<Expr> reduce() override;
};

Op::Op(shared_ptr<Token> tok, shared_ptr<Type> t)
	: Expr(tok, t){}

shared_ptr<Expr> Op::reduce()
{
	shared_ptr<Expr> x = gen();
	shared_ptr<Temp> t = std::make_shared<Temp>(type);   
	emit(t->toString() + " = " + x->toString());
	return t;
}
#endif