#ifndef TEMP_H
#define TEMP_H

#include"Expr.h"
#include"Word.h"

class Temp : public Expr
{
public:
	Temp(shared_ptr<Type>);

	static int count;
	int number = 0;
	virtual string toString() override;
};

int Temp::count = 0;

Temp::Temp(shared_ptr<Type> t)
	: Expr(Word::temp, t), number(++count) {}

string Temp::toString()
{
	return "t" + std::to_string(number);
}

#endif

