#ifndef _STACK_
#define _STACK_

#include "PointInt.h"
#include <vector>

class Stack{
	std::vector<PointInt> stack;

public :
	Stack();
	void push(PointInt p);
	PointInt pop();
	bool isEmpty();
};
#endif