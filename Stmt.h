#ifndef STMT_H
#define STMT_H

#include"Expr.h"

class Stmt : public Node
{
public:
	Stmt();

	virtual void gen(int, int);
	int begin = 0;
	 int after = 0;
	 // Null and Enclosing used for break- and continue stmts
	static  shared_ptr<Stmt> Null;
	static shared_ptr<Stmt> Enclosing;
};
shared_ptr<Stmt>Stmt::Null;
shared_ptr<Stmt>Stmt::Enclosing = Null;


Stmt::Stmt()
{}
// called with labels begin and after
void Stmt::gen(int b, int a)
{}
#endif
