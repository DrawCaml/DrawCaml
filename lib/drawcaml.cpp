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
#include "container.cpp"

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

//WINDOW.CPP

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


extern "C" value createWindow_cpp(value name, int posX, int posY, int sizeX, int sizeY) {
	const char* windowName = String_val(name);
	SWindow* win = new SWindow(windowName, posX, posY, sizeX, sizeY, 1);
	windows.push_back(win);

	win->draw();

  	// tests (uncomment)
	/*win->mContainer->setBgColor("yellow");
	SContainer* cont2 = new SContainer(SLayout::GridLayout, 5, 5);
	cont2->setBgColor("green");
	win->mContainer->addElem(cont2, 50, 50);
	
	SContainer* cont3 = new SContainer(SLayout::FloatLayout);
	cont3->setBgColor("red");
	SContainer* cont4 = new SContainer(SLayout::FloatLayout);
	cont4->setBgColor("blue");
	SContainer* cont5 = new SContainer(SLayout::FloatLayout);
	cont5->setBgColor("orange");
	SContainer* cont6 = new SContainer(SLayout::FloatLayout);
	cont6->setBgColor("purple");

	cont2->addElem(cont3);
	cont2->addElem(cont4);
	cont2->addElem(cont5, 3, 3);
	cont2->addElem(cont6, 1, 2);

  	win->draw();*/

	return caml_copy_nativeint((long)win); 
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

void test(vector<Argument> args) {
	LOG("WAOUF\n");
	return;
}*/

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

extern "C" value createContainer_cpp(value layout/*,value width, value height*/) {
	SLayout l = (SLayout) Nativeint_val(layout);
	// int w = Int_val(width);
	// int h = Int_val(height);
	SContainer* c = new SContainer(l/*,w,h*/);
	return caml_copy_nativeint((long)c);
}

extern "C" value setPos_cpp(value object,value posX,value posY) {
	SElement* e = (SElement *) Nativeint_val(object);
	int posx = Int_val(posX);
	int posy = Int_val(posY);

	if (!e) {
		WARNING("Element doesn't exist\n");
		return Val_unit;
	}
	SWindow* win = e->mWin;
	
	// TO BE GENERALIZED IN ACTION.cpp	
	mutex* m=new mutex;
	m->lock();
	Action action;	
	action.mResultLock = m;
	action.mArgs={posx,posy};
	// action.mFun.f2 = &(e->setPos); // DOESN'T WORK BECAUSE OF BOUNDED METHOD
	// sol: modify call and function storage in action to support class methods
	action.mFun.f2 = &SElement::setPos;
	//in ation:
	// when window calls action:
	mContainer/*pointer to container of window*/->*(action.mFun.f2);



	win->mActionMutex.lock();

	win->mSharedQueue.push(action);		

	sendDummyEvent(win);	

	win->mActionMutex.unlock();

	LOG("Sent message!\n");
	//on attend pas le message
	LOG("Message was processed\n");
	
	return Val_unit;
}

extern "C" value setSize_cpp(value object,value sizeX,value sizeY) {
	SElement* e = (SElement *) Nativeint_val(object);
	int sizex = Int_val(posX);
	int sizey = Int_val(posY);

	if (!e) {
		WARNING("Element doesn't exist\n");
		return Val_unit;
	}
	SWindow* win = e->mWin;
	
	mutex* m=new mutex;
	m->lock();
	Action action;	
	action.mResultLock = m;
	action.mArgs={sizex,sizey};
	action.mFun.f2 = &(e->setSize); 

	win->mActionMutex.lock();

	win->mSharedQueue.push(action);		

	sendDummyEvent(win);	

	win->mActionMutex.unlock();

	LOG("Sent message!\n");
	//on attend pas le message
	LOG("Message was processed\n");
	
	return Val_unit;
}

// Containers are the only objects where to which we can add elements
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
	
	mutex* m=new mutex;
	m->lock();
	Action action;	
	action.mResultLock = m;
	action.mArgs=vector<Argument>({e_add,posx,posy});
	action.mFun.f3 = &(e->addElem); 

	win->mActionMutex.lock();

	win->mSharedQueue.push(action);		

	sendDummyEvent(win);	

	win->mActionMutex.unlock();

	LOG("Sent message!\n");
	//on attend pas le message
	LOG("Message was processed\n");
	
	return Val_unit;
}

extern "C" value setBgColor_cpp(value object,value name) {
	// for now only containers can change their background
	// later -> More General Type
	SContainer* e = (SContainer *) Nativeint_val(object);
	const char* blase = String_val(name);

	if (!e) {
		WARNING("Element doesn't exist\n");
		return Val_unit;
	}
	SWindow* win = e->mWin;
	
	mutex* m=new mutex;
	m->lock();
	Action action;	
	action.mResultLock = m;
	action.mArgs={blase};
	action.mFun.f2 = &(e->setBgColor); 

	win->mActionMutex.lock();

	win->mSharedQueue.push(action);		

	sendDummyEvent(win);	

	win->mActionMutex.unlock();

	LOG("Sent message!\n");
	//on attend pas le message
	LOG("Message was processed\n");
	
	return Val_unit;
}