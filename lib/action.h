#pragma once

#include <vector>
#include <mutex>
#include <string>

#include "element.h"
#include "container.h"

using namespace std;

union Argument {
	int _int;
	SElement* _SEltPtr;
	SContainer* _SContPtr;
	// string _str;
};

union Function {
	void (*f)(void);
	void (*f1)(Argument);
	void (*f2)(Argument, Argument);
	void (*f3)(Argument, Argument, Argument);
	void (*fN)(vector<Argument>);
};

class Action {
	public:
		Function mFun;
		vector<Argument> mArgs;
		mutex* mResultLock;
		
		void Call();
};



