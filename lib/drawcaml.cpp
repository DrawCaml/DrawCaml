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

/**
 * All the functions in this file are unpacking parameters from Ocaml and either pushing an user action into the queue
 * (and waiting or not for the answer) or grabing directly some information about the window
 * 
 * For user actions we bind the method with its parameters so that the queue only contains void(void) functions
*/

/*
Window methods
*/

extern "C" value createWindow_cpp(value name, value vposX, value vposY, value vsizeX, value vsizeY) {
	const char* windowName = String_val(name);
	int posX = Int_val(vposX), posY = Int_val(vposY);
	int sizeX = Int_val(vsizeX), sizeY = Int_val(vsizeY);
	SWindow* win = new SWindow(windowName, posX, posY, sizeX, sizeY, 1);
	windows.push_back(win);

	win->draw();

	return caml_copy_nativeint((long)win); 
}

extern "C" value setWindowContainer_cpp(value window, value container){
	SWindow* win = (SWindow *) Nativeint_val(window);
	SContainer* cont = (SContainer *) Nativeint_val(container);
	win->mContainer = cont;
	cont->updateWin(win);
	cont->mPosX = 0;
	cont->mPosY = 0;
	cont->setSize(win->mWidth, win->mHeight);
	return Val_unit;
}

extern "C" value waitForClose_cpp(value window){
	SWindow* win = (SWindow *) Nativeint_val(window);
	LOG("Waiting for window '" + win->mName + "' to close\n");
	while(!win->mClosed) {
	}
	LOG("Window Closed\n");
	return Val_unit;
}

extern "C" value winNotClosed_cpp(value window){
	SWindow* win = (SWindow *) Nativeint_val(window);
	return Val_bool(!win->mClosed);
}

/*
 Element methods
*/

extern "C" value getPos_cpp(value object) {
	SElement* e = (SElement *) Nativeint_val(object);

	if (!e) {
		WARNING("Element doesn't exist\n");
		return Val_unit;
	}

	int posx = e->mPosX;
	int posy = e->mPosY;
	
	value pos = caml_alloc(2, 0);
	Store_field(pos, 0, Val_int(posx));
	Store_field(pos, 1, Val_int(posy));

	return pos;
}

extern "C" value getSize_cpp(value object,value sizeX,value sizeY) {
	SElement* e = (SElement *) Nativeint_val(object);

	if (!e) {
		WARNING("Element doesn't exist\n");
		return Val_unit;
	}

	int sizex = e->mSizeX;
	int sizey = e->mSizeY;
	
	value size = caml_alloc(2, 0);
	Store_field(size, 0, Val_int(sizex));
	Store_field(size, 1, Val_int(sizey));

	// fill pos with posx and posy

	return size;
}

extern "C" value setSize_cpp(value object,value sizeX,value sizeY) {
	SElement* e = (SElement *) Nativeint_val(object);
	int sx = Int_val(sizeX);
	int sy = Int_val(sizeY);

	if (!e) {
		WARNING("Element doesn't exist\n");
		return Val_unit;
	}

	SWindow* win = e->mWin;
	Action* a = new Action(win,bind(&SElement::setSize,e,sx,sy));
	/*if(win && a && a->mResultLock && !(win->is_Xlib)) {
		(a->mResultLock)->lock();
	}*/

	return Val_unit;
}

/*
Label methods
*/

extern "C" value createLabel_cpp(value text, value font) {
	const char* txt = String_val(text);
	const char* ft = String_val(font);
	SLabel* l = new SLabel();
	l->setText(txt);
	l->setFont(ft);
	return caml_copy_nativeint((long)l);
}

extern "C" value setText_cpp(value lbl, value text){
	SLabel* l = (SLabel*) Nativeint_val(lbl);
	const char* txt = String_val(text);
	SWindow* win = l->mWin;
	Action* a = new Action(win, bind(&SLabel::setText, l, txt));
	return Val_unit;
}

extern "C" value setLabelColor_cpp(value lbl, value col){
	SLabel* l = (SLabel*) Nativeint_val(lbl);
	const char* color = String_val(col);
	SWindow* win = l->mWin;
	Action* a = new Action(win, bind(&SLabel::setColor, l, color));
	return Val_unit;
}

extern "C" value setLabelFont_cpp(value lbl, value ft){
	SLabel* l = (SLabel*) Nativeint_val(lbl);
	const char* font = String_val(ft);
	SWindow* win = l->mWin;
	Action* a = new Action(win, bind(&SLabel::setFont, l, font));
	return Val_unit;
}


/*
 Container methods
*/

extern "C" value createContainer_cpp(value layout, value width, value height) {
	SLayout l = (SLayout) Int_val(layout);
	int w = Int_val(width);
	int h = Int_val(height);
	SContainer* c = new SContainer(l,w,h);
	return caml_copy_nativeint((long)c);
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
	//waiting for the end of the action to resume
	if(win && a && a->mResultLock && !(win->is_Xlib)) {
		(a->mResultLock)->lock();
	}

	return Val_unit;
}

extern "C" value removeElem_cpp(value object,value object_del) {
	SContainer* e = (SContainer *) Nativeint_val(object);
	SElement* e_del = (SElement *) Nativeint_val(object_del);

	if ((!e)||(!e_del)) {
		WARNING("Element doesn't exist\n");
		return Val_unit;
	}
	SWindow* win = e->mWin;

	Action* a = new Action(win,bind(&SContainer::removeElem,e,e_del));
	
	if(win && a && a->mResultLock && !(win->is_Xlib)) {
		(a->mResultLock)->lock();
	}
	
	return Val_unit;
}

extern "C" value setBgColor_cpp(value object,value color) {
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

extern "C" value setWindowEventHandler_cpp(value window, value f){
	SWindow* win = (SWindow *) Nativeint_val(window);
	win->mEventHandler = f;
	return Val_unit;
}
