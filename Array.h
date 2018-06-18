#ifndef ARRAY_H
#define ARRAY_H

#include"Type.h"

class Array : public Type
{
public:
	Array(int, shared_ptr<Type>);
	// type of array  
	shared_ptr<Type> of;
	// number of elements
	int size = 1;

	 string toString();
};

Array::Array(int sz, shared_ptr<Type> t)
	: Type("[]", Tag::INDEX, (sz * (t->width))), size(sz), of(t){}

string Array::toString()
{
	return "[" + std::to_string(size) + "]" + of->toString();
}

#endif