#ifndef WORD_H
#define WORD_H
#include"Token.h"

#include<memory>
using std::shared_ptr;

class Word : public Token
{
public:
	Word(string s, int tag);

	string lexeme;
	 string toString() { return lexeme; }

	static shared_ptr<Word> and, or, eq, ne, le, ge, minus, True, False, temp;
};
shared_ptr<Word> Word::and = std::make_shared<Word>("&&", Tag::AND);
shared_ptr<Word> Word::or = std::make_shared<Word>("||", Tag::OR);
shared_ptr<Word> Word::eq = std::make_shared<Word>("==", Tag::EQ);
shared_ptr<Word> Word::ne = std::make_shared<Word>("!=", Tag::NE);
shared_ptr<Word> Word::le = std::make_shared<Word>("<=", Tag::LE);
shared_ptr<Word> Word::ge = std::make_shared<Word>(">=", Tag::GE);
shared_ptr<Word> Word::minus = std::make_shared<Word>("minus", Tag::MINUS);
shared_ptr<Word> Word::True = std::make_shared<Word>("true", Tag::TRue);
shared_ptr<Word> Word::False = std::make_shared<Word>("false", Tag::FAlse);
shared_ptr<Word> Word::temp = std::make_shared<Word>("t", Tag::TEMP);

Word::Word(string s, int tag)
	: Token(tag), lexeme(s) {}

#endif