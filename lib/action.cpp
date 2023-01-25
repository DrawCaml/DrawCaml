#include "action.h"
#include "window.h"
#include <cstring>

using namespace std;

using Function = function<void(void)>;

void sendDummyEvent(SWindow* win) {
	// https://stackoverflow.com/questions/8592292/how-to-quit-the-blocking-of-xlibs-xnextevent
	XClientMessageEvent dummyEvent;
	memset(&dummyEvent, 0, sizeof(XClientMessageEvent));
	dummyEvent.type = ClientMessage;
	dummyEvent.window = win->mWindow;
	dummyEvent.format = 32;
	XSendEvent(win->mDisplay, win->mWindow, 0, 0, (XEvent*)&dummyEvent);
	XFlush(win->mDisplay);
	return;
}

Action::Action(SWindow* win, Function f){
	// printf("%d\n", win);
	if(win && !win->mClosed){ // avoid segfaults when window is closed
		mutex* m = new mutex;
		m->lock();
		mResultLock = m;
		mFun = f;

		win->mActionMutex.lock();

		win->mSharedQueue.push(*this);

		sendDummyEvent(win);

		win->mActionMutex.unlock();

		LOG("Sent message!\n");
	} else {
		f();
		mResultLock = NULL;
	}
}


void Action::Call() {
	mFun();
	mResultLock->unlock();
	LOG("Message was processed\n");
	return;
}
