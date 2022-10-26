#define CAML_NAME_SPACE
#include "caml/mlvalues.h"
#include "caml/alloc.h"
#include "caml/memory.h"
#include "caml/fail.h"
#include "caml/callback.h"
#include "caml/custom.h"
#include "caml/intext.h"
#include "caml/threads.h"


#include <stdio.h>
#include <stdlib.h>
#include <cstring>

#include <X11/Xlib.h>
#include <unistd.h>
#include "window.h"
#include "utils.h"
#include "action.h"
#include <iostream>


extern "C" value createWindow_cpp(value name) {
	const char* windowName = String_val(name);
	SWindow* win = new SWindow(windowName, 10, 10, 500, 500, 1);
  	win->draw();
	return caml_copy_nativeint((long)win); 
}

void test(vector<Argument> args) {
	LOG("WAOUF\n");
	return;
}

extern "C" value sendMessage_cpp(value window, value message) {
	SWindow* win = (SWindow *) Nativeint_val(window);
	int val = Int_val(message);

	if (!win or win->mClosed) {
		WARNING("Can't send message to a closed windows!\n");
		return Val_unit;
	}

	win->mActionMutex.lock();
	
	mutex* m=new mutex;
	m->lock();
	Action action;	
	action.mResultLock = m;
	action.mArgs={};
	action.mFun.fN = &test; 

	win->mSharedQueue.push(action);		

	// https://stackoverflow.com/questions/8592292/how-to-quit-the-blocking-of-xlibs-xnextevent
	XClientMessageEvent dummyEvent;
	memset(&dummyEvent, 0, sizeof(XClientMessageEvent));
	dummyEvent.type = ClientMessage;
	dummyEvent.window = win->mWindow;
	dummyEvent.format = 32;
	XSendEvent(win->mDisplay, win->mWindow, 0, 0, (XEvent*)&dummyEvent);
	XFlush(win->mDisplay);
	win->mActionMutex.unlock();

	LOG("Sent message!\n");
	m->lock();
	m->unlock();
	LOG("Message was processed\n");
	
	return Val_unit;
}

extern "C" value waitForClose_cpp(value window){
	SWindow* win = (SWindow *) Nativeint_val(window);
	LOG("Waiting for window '" + win->mName + "' to close\n");
	while(!win->mClosed) {
		continue;
	}
	LOG("Window Closed\n");
	return Val_unit;
}
