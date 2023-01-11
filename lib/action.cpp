#include "action.h"


// to modify
void Action::Call() {
	switch (mArgs.size()) {
		case 0:
			mFun.f();
			break;
		case 1:
			mFun.f1(mArgs[0]);
			break;
		case 2:
			mFun.f2(mArgs[0], mArgs[1]);
			break;
		case 3:
			mFun.f3(mArgs[0], mArgs[1], mArgs[2]);
			break;
		default:
			mFun.fN(mArgs);
	}

}
