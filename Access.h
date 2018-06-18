#ifndef ACCESS_H
#define ACCESS_H

#include"Op.h"
#include"Id.h"

class Access : public Op
{
public:
	Access(shared_ptr<Id>, shared_ptr<Expr>, shared_ptr<Type>);

	virtual shared_ptr<Expr> gen() override;
	virtual void jumping(int, int) override;
	virtual string toString() override;

	shared_ptr<Id>arr;
	shared_ptr<Expr>index;
};
// t is element type after
// flattening the array
Access::Access(shared_ptr<Id>a, shared_ptr<Expr>i, shared_ptr<Type>t)
	:Op(std::make_shared<Word>("[]", Tag::INDEX), t), arr(a), index(i){}

shared_ptr<Expr> Access::gen()
{
	return std::make_shared<Access>(arr, index->reduce(), type);
}

void Access::jumping(int t, int f)
{
	emitJumps(reduce()->toString(), t, f);         
}

string Access::toString()
{
	return arr->toString() + " [ " + index->toString() + " ] ";
}

#endif