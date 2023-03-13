#include "action.h"
#include "window.h"
#include <cstring>

using namespace std;

using Function = function<void(void)>;

void sendDummyEvent(SWindow* win) {
	/**
	 * sends a dummy event to the Xlib thread so that it looks into the action queue 
	*/
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
	/**
	 * constructor that directly pushes the action into the queue
	*/
	if(win && !win->mClosed){ // avoid segfaults when window is closed
		mutex* m = new mutex;
		m->lock();
		mResultLock = m;
		mFun = f;

		win->mActionMutex.lock();
		//pushes the action into the queue
		win->mSharedQueue.push(*this);
		//tell the Xlib thread that we have pushed an action to the queue for it to execute
		sendDummyEvent(win);

		win->mActionMutex.unlock();

		LOG("Sent message!\n");
	} else {
		f();
		mResultLock = NULL;
	}
}


void Action::Call() {
	/**
	 * just call the void(void) function of the action
	*/
	mFun();
	mResultLock->unlock();
	LOG("Message was processed\n");
	return;
}
