#ifndef NODE_H
#define NODE_H

#include<iostream>
using std::cout;
#include<windows.h>
#include<fstream>
using std::ios_base;
#include"Lexer.h"

class Node
{
public:
	Node();
	virtual ~Node();

	int lexline;
	static int labels;

	int newLabel();
	void emitLabel(int);
	void emit(string);

	void error(string);

	
private:
	HWND hwnd;
};

int Node::labels = 0;

Node::Node()
	:lexline(Lexer::line)
{
	//HWND hwnd = FindWindowEx(NULL, NULL, "Poppad", NULL);
 hwnd = GetForegroundWindow();
}

Node::~Node()
{}

int Node::newLabel()
{
	return ++labels;
}
void Node::emitLabel(int i)
{
	std::ofstream Code;
  Code.open("code.txt", std::ios::app);
  if (!Code.is_open())
  {
	  error("file not could be opened");
	  exit(1);
  }
  
  Code << "   L" << i << ":";
	Code.close();
}

void Node::emit(string s)
{
	std::ofstream Code;
 Code.open("code.txt", std::ios::app);
 if (!Code.is_open())
 {
	 error("file not could be opened");
	 exit(1);
 }
	Code << "   " << s << "\n";
	Code.close();
}

void Node::error(string s)
{
	string message = "near line +" + std::to_string(lexline) + "+: ";
	s = message + s;
	MessageBox(hwnd, s.c_str(), "untitled", MB_OK | MB_ICONERROR);
	exit(0);
}
#endif