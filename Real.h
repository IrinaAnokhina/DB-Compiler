#ifndef REAL_H
#define REAL_H

#include"Token.h"

class Real : public Token
{
public:
	Real(float v);

	float value;
  string toString() { return std::to_string(value); }
};

Real::Real(float v)
	: Token(Tag::REAL), value(v){}
#endif