#ifndef LEXER_H
#define LEXER_H

#include<map>
using std::map;
#include<string>
using std::string;

#include"Test.h"
#include"Word.h"
#include"Num.h"
#include"Real.h"
#include"Type.h"

class Token;

class Lexer
{
public:
	Lexer(shared_ptr<Test>);
	~Lexer();
	static int line;
	static int symbol;
	static char peek;
	shared_ptr<Test> curText;
	map<string, shared_ptr<Word>> words;                      

	void reserve(shared_ptr<Word> w) {
		words.insert(std::make_pair(w->lexeme, w));
	}
	void readch();
	bool readch(char);
	shared_ptr<Token> scan();
};
int Lexer::line = 1;
int Lexer::symbol = 0;
char Lexer::peek = ' ';

Lexer::Lexer(shared_ptr<Test>t)
	: curText(t)
{
	reserve(std::make_shared<Word>("if", Tag::IF));
	reserve(std::make_shared<Word>("else", Tag::ELSE));
	reserve(std::make_shared<Word>("while", Tag::WHILE));
	reserve(std::make_shared<Word>("do", Tag::DO));
	reserve(std::make_shared<Word>("break", Tag::BREAK));
	reserve(std::make_shared<Word>("for", Tag::FOR));
	reserve(std::make_shared<Word>("continue", Tag::CONTINUE));
	reserve(Word::True);
	reserve(Word::False);
	reserve(Type::Int);
	reserve(Type::Float);
	reserve(Type::Char);
	reserve(Type::Bool);
}
Lexer::~Lexer()
{
	symbol = 0;
	line = 1;
}
void Lexer::readch()
{
	peek = curText->text[symbol];
	++symbol;
}

bool Lexer::readch(char c)
{
	readch();
	if (peek != c)
		return false;
	peek = ' ';
	return true;
}

 shared_ptr<Token> Lexer::scan()
{
for(;;	readch())
{
		if (peek == ' ' || peek == '\t' || peek == '\r')
			continue;
		else if (peek == '\n')
			++line;
		else
			break;
	} 
	switch (peek)
	{
	case '&':
		if (readch('&'))
			return Word::and;
		else
			return std::make_shared<Token>('&');
	case '|':
		if (readch('|'))
			return Word::or;
		else
			return std::make_shared<Token>('|');
	case '=':
		if (readch('='))
			return Word::eq;
		else
			return std::make_shared<Token>('=');
	case '!':
		if (readch('='))
			return Word::ne;
		else
			return std::make_shared<Token>('!');
	case '<':
		if (readch('='))
			return Word::le;
		else
			return std::make_shared<Token>('<');
		
	case '>':
		if (readch('='))
			return Word::ge;
		else
			return std::make_shared<Token>('>');
	default:
		break;
	}
	if (isdigit(peek))
	{
		int v = 0;
		do {
			v = 10 * v + ((int)peek - (int)'0');
			readch();
		} while (isdigit(peek));
		if (peek != '.')
			return std::make_shared<Num>(v);
	
		float x = (float)v;
		float d = 10.0;
		for (;;)
		{
			readch();
			if (!(isdigit(peek)))
				break;
			x += ((int)peek - (int)'0') / d;
			d *= 10.0;
		}
		return std::make_shared<Real>(x);
	}
	if (isalpha(peek))
	{
		string buffer;
		do{
			buffer += peek;
			readch();
		} while (isalpha(peek));
		std::map<string, shared_ptr<Word>>::iterator location;
		location = words.find(buffer);
		if (location != words.end())
			return location->second;
		
		shared_ptr<Word> w = std::make_shared<Word>(buffer, Tag::ID);
		words.insert(std::make_pair(buffer, w));
		return  w;
	} 
	shared_ptr<Token> tok = std::make_shared<Token>(peek);
	peek = ' ';
	return tok;
}
#endif