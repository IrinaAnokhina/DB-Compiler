#ifndef SEQ_H
#define SEQ_H

#include"Stmt.h"
//Stmt -> Stmt1 Seq | null
//Seq -> Stmt1 Stmt2

class Seq : public Stmt
{
public:
	Seq(shared_ptr<Stmt>, shared_ptr<Stmt>);

	virtual void gen(int, int) override;

	shared_ptr<Stmt>stmt1;
	shared_ptr<Stmt> stmt2;
};

Seq::Seq(shared_ptr<Stmt>s1, shared_ptr<Stmt>s2)
	: stmt1(s1), stmt2(s2){}

void Seq::gen(int b, int a)
{
	if (stmt1 == Stmt::Null)
		stmt2->gen(b, a);
		
	else if (stmt2 == Stmt::Null)
		stmt1->gen(b, a);
		
	else
	{
		int label = newLabel();
		stmt1->gen(b, label);
		emitLabel(label);
		stmt2->gen(label, a);
	}
}

#endif