#ifndef SETEL_H
#define SETEL_H

#include"Stmt.h"
#include"Access.h"
#include"Array.h"
//a[i] = Expr

class SetElem : public Stmt
{
public:
	SetElem(shared_ptr<Access>, shared_ptr<Expr>);

	shared_ptr<Type>check(shared_ptr<Type>, shared_ptr<Type>);
	virtual void gen(int, int) override;

	shared_ptr<Id>arr;
	shared_ptr<Expr>index;
	shared_ptr<Expr> expr;
};

SetElem::SetElem(shared_ptr<Access>ac, shared_ptr<Expr>x)
	: arr(ac->arr), index(ac->index), expr(x)
{
	if (check(ac->type, expr->type) == nullptr)
		error("type error");
}

shared_ptr<Type>SetElem::check(shared_ptr<Type>t1, shared_ptr<Type>t2)
{
	shared_ptr<Array> ar1 = dynamic_pointer_cast<Array>(t1);
	shared_ptr<Array> ar2 = dynamic_pointer_cast<Array>(t2);
	//check for "array = j" will not be execute 
	if (ar1 != 0 || ar2 != 0)
		return nullptr;
	//type check
	else if (t1 == t2)
		return t2;
	else if (Type::numeric(t1) && Type::numeric(t2)) // !!!! this allow to array.int = id.float !!!
		return t2;
	else
		return nullptr;
}

void SetElem::gen(int b, int a)
{
	string s1 = index->reduce()->toString();
	string s2 = expr->reduce()->toString();
	emit(arr->toString() + " [ " + s1 + " ] = " + s2);
}

#endif