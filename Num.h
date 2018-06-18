#ifndef NUM_H
#define NUM_H
#include"Token.h"

class Num : public Token
{
public:
	Num(int);
	int value;

	 string toString() { return std::to_string(value); }
};

Num::Num(int v)
	: Token(Tag::NUM), value(v){}
#endif