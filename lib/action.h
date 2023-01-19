#pragma once

#include <variant>
#include <vector>
#include <mutex>
#include <string>

#include "element.h"
#include "container.h"

using namespace std;
//using function = variant<function<void(int,int)>,function<void(SElement*,int,int)>>;
//list of all possible function pointer types
// using Function = variant<void(SContainer::*)(int,int),void(SContainer::*)(SElement*,int,int)>;
//using Argument = variant<int,SElement*>;

union Argument {
	int _int;
	SElement* _SEltPtr;
	SContainer* _SContPtr;
	// string _str;
};

//using PObject = variant<SContainer*>;

// a, b, c;
// std::function<void(void)> f = std::bind(&Foo::doSomethingArgs, this, a, b, c);
// f = lambda x, y: this.something(x, y)
// f()



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
		//vector<Argument> mArgs;
		//PObject mPObj;
		mutex* mResultLock;
		
		void Call();
};



