#pragma once
#include<memory>
using std::dynamic_pointer_cast;
#include"Stmt.h"
#include"Set.h"

//for(S1; B; S2;) S3
class For : public Stmt                            
{
public:
	For(shared_ptr<Stmt>s1, shared_ptr<Expr>x, shared_ptr<Stmt>s2, shared_ptr<Stmt>s3);

	void init(shared_ptr<Stmt>s1, shared_ptr<Expr>x, shared_ptr<Stmt>s2, shared_ptr<Stmt>s3);
	virtual void gen(int, int) override;
private:
	shared_ptr<Stmt>init_val;
	shared_ptr<Expr>condition;
	shared_ptr<Stmt>step;
	shared_ptr<Stmt>instruction;
	shared_ptr<Id>variable;
};

For::For(shared_ptr<Stmt>s1, shared_ptr<Expr>x, shared_ptr<Stmt>s2, shared_ptr<Stmt>s3)
	:init_val(s1), condition(x), step(s2), instruction(s3)
{}

void For::init(shared_ptr<Stmt>s1, shared_ptr<Expr>x, shared_ptr<Stmt>s2, shared_ptr<Stmt>s3)
{
	shared_ptr<Set>temp = dynamic_pointer_cast<Set>(s1);
	if (temp == nullptr)
		error("type error in for");
	if (temp->id->type != Type::Int)
		error("requaired 'int' for step-variable in For");

	init_val = s1;
	variable = temp->id;

	if (x->type != Type::Bool)
		error("requaired 'bool' for condition expression in For");

	condition = x;

	shared_ptr<Set>temp2 = dynamic_pointer_cast<Set>(s2);
	if (temp2 == nullptr)
		error("type error in for");
	if (temp2->id != variable)
		error("step variable don`t match");

	step = s2;
	instruction = s3;
}

void For::gen(int b, int a)
{
	after = a;
	init_val->gen(b, a);
	int label = newLabel();					
	//start of B
	int label2 = newLabel();
	begin = label2;
	emitLabel(label);
	condition->jumping(0, a);				
	//B.true = fall    B.false = after
	instruction->gen(label, label2);		
	// S2.next = S3 
	emitLabel(label2);						
	//start of S2 (step)
	step->gen(label2, label);				
	// S3.next = begin of S2
	emit("goto L" + std::to_string(label));
}

