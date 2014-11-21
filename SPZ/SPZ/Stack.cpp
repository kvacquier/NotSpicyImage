#include "Stack.h"
#include "PointInt.h"
#include <vector>

Stack::Stack()
{
}
void Stack::push(PointInt p)
{
	this->stack.push_back(p);
}
PointInt Stack::pop()
{
	PointInt p = this->stack.back();
	this->stack.pop_back();
	return p;
};
bool Stack::isEmpty()
{
	if(this->stack.size() == 0)
		return true;
	return false;
};