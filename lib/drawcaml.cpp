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
#include <iostream>


SWindow* win;

extern "C" value createWindow_cpp(value unit) {
	win = new SWindow("test DrawCaml", 10, 10, 500, 500, 1);
  	win->draw();

	return Val_unit;
}

extern "C" value sendMessage_cpp(value message) {
	int val = Int_val(message);

	win->mActionMutex.lock();

	win->mSharedQueue.push(val);		

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
	return Val_unit;
}

extern "C" value waitForClose_cpp(value unit){
	while(!win->mClosed) {
		continue;
	}
	LOG("Window Closed\n");
	return Val_unit;
}
