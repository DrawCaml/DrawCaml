#include "action.h"
#include <variant>

// VARIANT
// template <class ... P> struct Combine : P... {
//    Combine(P... ps) : P{ ps }... {
//    }
//    //
//    // le using qui suit est nécessaire, mais peut ne pas être
//    // supporté sur certains compilateurs en 2018
//    //
//    using P::operator()...;
// };
// template <class ... F>
//    Combine<F...> combine(F... fs) {
//       return { fs ... };
//    }

// to modify
void Action::Call() {

	mFun();

	mResultLock->unlock();
// VARIANT
	// visit(combine(
	// 	[this](void(*setPos)(int,int)) {(*(get<0>(mPObj)).*(get<0>(mFun)))(get<0>(mArgs[0]),get<0>(mArgs[1]));}
	// 	[this](void(*addElem)(SElement*,int,int)) {*mPObj.*mFun(mArgs[0],mArgs[1],mArgs[2])}
	// 	)
	// 	,mFun);
	
// OLD
	//2 cases if class methode or not ?
	/*switch (mArgs.size()) {
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
	}*/
	return;

}
