#ifndef TYPE_H
#define TYPE_H

#include"Word.h"

class Type : public Word
{
public:
	Type(string, int, int);

	int width = 0;
	static bool numeric(shared_ptr<Type>);
	static shared_ptr<Type> maximum(shared_ptr<Type>, shared_ptr<Type>);
	
	static shared_ptr<Type>Int;
	static shared_ptr<Type> Float;
	static shared_ptr<Type>Char;
	static shared_ptr<Type>Bool;
};

shared_ptr<Type> Type::Int = std::make_shared<Type>("int", Tag::BASIC, 4);
shared_ptr<Type> Type::Float = std::make_shared<Type>("float", Tag::BASIC, 8);
shared_ptr<Type> Type::Char = std::make_shared<Type>("char", Tag::BASIC, 1);
shared_ptr<Type> Type::Bool = std::make_shared<Type>("bool", Tag::BASIC, 1);

Type::Type(string s, int tag, int w)
	: Word(s, tag), width(w){}

bool Type::numeric(shared_ptr<Type> t)
{
		if (t == Char || t == Int || t == Float)
			return true;
		else
			return false;
}

shared_ptr<Type> Type::maximum(shared_ptr<Type> t1, shared_ptr<Type> t2)
{
	
	if (!(numeric(t1)) || !(numeric(t2)))
		return nullptr;
	else if (t1 == Float || t2 == Float)
		return Float;
	else if (t1 == Int || t2 == Int)
		return Int;
	else
		return Char;
}
#endif