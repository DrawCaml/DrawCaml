#pragma once

#include <variant>
#include <vector>
#include <mutex>
#include <functional>
#include "container.h"
#include "element.h"


using namespace std;
//using function = variant<function<void(int,int)>,function<void(SElement*,int,int)>>;
//list of all possible function pointer types
using Function = variant<void(SContainer::*)(int,int),void(SContainer::*)(SElement*,int,int)>;

using Argument = variant<int,SElement*>;

using PObject = variant<SContainer*>;



/*union Function {
	void (*f)(void);
	void (*f1)(Argument);
	void (*f2)(Argument, Argument);
	void (*f3)(Argument, Argument, Argument);
	void (*fN)(vector<Argument>);
};*/

class Action {
	public:
		Function mFun;
		vector<Argument> mArgs;
		PObject mPObj;

		mutex* mResultLock;
		
		void Call();
};



