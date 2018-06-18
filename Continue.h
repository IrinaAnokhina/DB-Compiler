#pragma once
#include<typeinfo>
#include"Stmt.h"

class Continue : public Stmt
{
public:
	Continue();

	virtual void gen(int, int) override;
private:
	shared_ptr<Stmt>stmt;
};

Continue::Continue()
{
	if (Stmt::Enclosing == nullptr)
		error("unenclosed continue");
	stmt = Stmt::Enclosing; 
	//save enclosing structure
}

void Continue::gen(int b, int a)
{
	emit("goto L" + std::to_string(stmt->begin));
	stmt.reset();
}

