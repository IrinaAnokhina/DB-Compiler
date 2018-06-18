#ifndef EXPR_H
#define EXPR_H

#include"Node.h"
class Token;
class Type;

class Expr : public Node, public std::enable_shared_from_this<Expr>
{
public:
	Expr(shared_ptr<Token>, shared_ptr<Type>);

	virtual  shared_ptr<Expr> gen();
	virtual  shared_ptr<Expr> reduce();

	virtual void jumping(int, int);
	void emitJumps(string, int, int);
	 shared_ptr<Expr> getptr();
	virtual string toString();

	shared_ptr<Token> op;
	shared_ptr<Type> type;

};

Expr::Expr(shared_ptr<Token> tok, shared_ptr<Type> t)
	: op(tok), type(t){}

 shared_ptr<Expr> Expr::getptr()
{
	return shared_from_this();
}

  shared_ptr<Expr> Expr::gen()
{
	return getptr();
}

 shared_ptr<Expr> Expr::reduce()
{
	return getptr();
}

void Expr::jumping(int t, int f)
{
	emitJumps(toString(), t, f);
}

void Expr::emitJumps(string test, int t, int f)
{
	if (t != 0 && f != 0)
	{
		emit("if " + test + " goto L" + std::to_string(t));
		emit("goto L" + std::to_string(f));
	}
	else if (t != 0)
		emit("if " + test + " goto L" + std::to_string(t));
	else if (f != 0)
		emit("iffalse " + test + " goto L" + std::to_string(f));
	else;
}

string Expr::toString()
{
	return op->toString();
}

#endif