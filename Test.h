#ifndef TEST_H
#define TEST_H
#include<string>
using std::string;

class Test
{
public:
	Test(string);

	string text;

};

Test::Test(string t)
	:text(t){}
#endif