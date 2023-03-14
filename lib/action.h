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

/**
 * Object consisting of actions to be performed on the window, stored in window's queue.
 */
class Action {
	public:
		/**
		 * void(void) function with all parameters already binded
		 */
		Function mFun;

		/**
		 * lock to wait for the result of the action
		 */
		mutex* mResultLock;
		
		/**
		 * The constructor of Action adds it directly to the queue of the given window. (if not null)
		 */
		Action(SWindow* win, Function f);

		/**
		 * Performs the action (calls the function f given in the constructor). 
		 * Called by window's queue.
		 */
		void Call();
};



