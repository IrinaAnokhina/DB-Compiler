#ifndef ID_H
#define ID_H

#include"Expr.h"
class Word;
class Type;

class Id : public Expr
{
public:
	Id(shared_ptr<Word>, shared_ptr<Type>, int);
	// relative address
	int offset;
};

Id::Id(shared_ptr<Word> w, shared_ptr<Type> t, int b)
	: Expr(w, t), offset(b){}

#endif