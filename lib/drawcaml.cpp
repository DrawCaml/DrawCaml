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

using Function = function<void(void)>;

// keeps trace of c++ windows
std::vector<SWindow*> windows;

// to be added: a function to delete window objects


extern "C" value createWindow_cpp(value name, value vposX, value vposY, value vsizeX, value vsizeY) {
	const char* windowName = String_val(name);
	int posX = Int_val(vposX), posY = Int_val(vposY);
	int sizeX = Int_val(vsizeX), sizeY = Int_val(vsizeY);
	SWindow* win = new SWindow(windowName, posX, posY, sizeX, sizeY, 1);
	// printf("%d %d %d %d\n", posX, posY, sizeX, sizeY);
	windows.push_back(win);

	win->draw();

	return caml_copy_nativeint((long)win); 
}

extern "C" value setWindowContainer_cpp(value window, value container){
	SWindow* win = (SWindow *) Nativeint_val(window);
	SContainer* cont = (SContainer *) Nativeint_val(container);
	win->mContainer = cont;
	cont->updateWin(win);
	cont->setPos(0, 0);
	cont->setSize(win->mWidth, win->mHeight);
	return Val_unit;
}

extern "C" value draw_cpp(value window) {//toujours pour la window !!!, l'utilisateur ne fait jamais de call draw pour un container
	SWindow* win = (SWindow *) Nativeint_val(window);

	if (!win) {
		WARNING("Window doesn't exist\n");
		return Val_unit;
	}
	win->draw();
	return Val_unit;
}

extern "C" value sendMessage_cpp(value window, value message) {
	SWindow* win = (SWindow *) Nativeint_val(window);
	int val = Int_val(message);

	if (!win or win->mClosed) {
		WARNING("Can't send message to a closed windows!\n");
		return Val_unit;
	}

	/*win->mActionMutex.lock();
	
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
	LOG("Message was processed\n");*/
	
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


//CONTAINER.CPP

extern "C" value createContainer_cpp(value layout, value width, value height) {
	SLayout l = (SLayout) Int_val(layout);
	int w = Int_val(width);
	int h = Int_val(height);
	SContainer* c = new SContainer(l,w,h);
	return caml_copy_nativeint((long)c);
}

extern "C" value setPos_cpp(value object,value posX,value posY) {
	SContainer* e = (SContainer *) Nativeint_val(object);
	int posx = Int_val(posX);
	int posy = Int_val(posY);

	if (!e) {
		WARNING("Element doesn't exist\n");
		return Val_unit;
	}
	SWindow* win = e->mWin;
	
	Action* a = new Action(win,bind(&SContainer::setPos,e,posx,posy));	

	return Val_unit;
}

extern "C" value setSize_cpp(value object,value sizeX,value sizeY) {
	SElement* e = (SElement *) Nativeint_val(object);
	int sizex = Int_val(sizeX);
	int sizey = Int_val(sizeY);

	if (!e) {
		WARNING("Element doesn't exist\n");
		return Val_unit;
	}
	SWindow* win = e->mWin;
	
	Action* a = new Action(win,bind(&SElement::setSize,e,sizeX,sizeY));

	return Val_unit;
}

// Containers are the only objects to which we can add elements
extern "C" value addElem_cpp(value object,value object_added,value posX,value posY) {
	SContainer* e = (SContainer *) Nativeint_val(object);
	int posx = Int_val(posX);
	int posy = Int_val(posY);
	SElement* e_add = (SElement *) Nativeint_val(object_added);

	if ((!e)||(!e_add)) {
		WARNING("Element doesn't exist\n");
		return Val_unit;
	}
	SWindow* win = e->mWin;

	Action* a = new Action(win,bind(&SContainer::addElem,e,e_add,posx,posy));
	
	return Val_unit;
}


extern "C" value setBgColor_cpp(value object,value color) {
	// for now only containers can change their background
	// later -> More General Type
	SContainer* e = (SContainer *) Nativeint_val(object);
	const char* col = String_val(color);

	if (!e) {
		WARNING("Element doesn't exist\n");
		return Val_unit;
	}
	SWindow* win = e->mWin;

	Action* a = new Action(win,bind(&SContainer::setBgColor,e,col));
	
	return Val_unit;
}
