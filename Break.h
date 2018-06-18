#ifndef BREAK_H
#define BREAK_H

#include<typeinfo>
#include"Stmt.h"

class Break : public Stmt
{
public:
	Break();

	virtual void gen(int, int) override;

	shared_ptr<Stmt>stmt;
};

Break::Break()
{
	if (Stmt::Enclosing == NULL)
		error("unenclosed break");
	stmt = Stmt::Enclosing;
	//save enclosing structure
}

void Break::gen(int b, int a)
{
	emit("goto L" + std::to_string(stmt->after));
	stmt.reset();
}

#endif