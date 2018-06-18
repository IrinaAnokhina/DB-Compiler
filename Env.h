#ifndef ENV_H
#define ENV_H

#include<map>
using std::multimap;
#include"Id.h"
class Token;
class Id;

class Env
{
public:
	Env(shared_ptr<Env>);               

	void put(shared_ptr<Token>, shared_ptr<Id>);
 shared_ptr<Id> get(const shared_ptr<Token>&);

protected:
	shared_ptr<Env>prev;
private:
	multimap<shared_ptr<Token>, shared_ptr<Id>>table;
};

Env::Env(shared_ptr<Env>n)
{
	prev = n;
}

void Env::put( shared_ptr<Token> w, shared_ptr<Id> i)
{
	table.insert(std::make_pair(w, i));
}

 shared_ptr<Id> Env::get(const shared_ptr<Token> &w)
{
	//std::multimap<shared_ptr<Token>, shared_ptr<Id>>::iterator found;
	 auto found = table.find(w);
	if (found != table.end())
			return found->second;
	else
	{
		if (prev != nullptr)
		{
			shared_ptr<Env>cur = prev;
			while (cur != nullptr)
			{

				found = cur->table.find(w);
				if (found != cur->table.end())
					return found->second;
				else
					cur = cur->prev;
			}
		}
	}
	return nullptr;
}
#endif