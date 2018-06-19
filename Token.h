#ifndef TOKEN_H
#define TOKEN_H

#include<string>
using std::string;

#include"Tag.h"

class Token
{
public:
	Token(int);
	virtual ~Token();

	int tag;
	virtual string toString();
};

Token::Token(int t)
	: tag(t){}

Token::~Token()
{}

string Token::toString()
{
	string temp;
	//temp.clear();
 return (char)(tag) + temp;
}
#endif