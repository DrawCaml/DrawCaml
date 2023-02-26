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

class Action {
	public:
		Function mFun;
		mutex* mResultLock;
		
		Action(SWindow* win, Function f);
		void Call();
};



