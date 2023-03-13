#pragma once

#include <variant>
#include <vector>
#include <mutex>
#include <functional>
#include "container.h"
#include "element.h"

using namespace std;

using Function = function<void(void)>;

void sendDummyEvent(SWindow* win);

// class for user action
class Action {
	public:
		Function mFun;//void(void) function will all parameters already binded
		mutex* mResultLock;// lock to wait for the result of the action
		
		Action(SWindow* win, Function f);
		void Call();
};



