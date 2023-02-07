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
#include <iostream>
#include <unistd.h>
#include <thread>
#include <mutex>
#include <cstring>

#include <X11/Xlib.h>
#include "window.h"
#include "container.h"
#include "utils.h"

using namespace std;


SWindow::SWindow(string name, int posX, int posY, int width, int height, int borderSize){
 	XInitThreads();
	mName = name;
	mPosX = posX;
	mPosY = posY;
	mWidth = width;
	mHeight = height;
	mBorderSize = borderSize;
	mClosed = false;
	mThread = NULL;

	mDisplay = XOpenDisplay(NULL);
	if (!mDisplay){
		ERROR("Cannot open display!\n");
		exit(1);
 	}

 	mScreen = DefaultScreen(mDisplay);

 	mWindow = XCreateSimpleWindow(mDisplay, RootWindow(mDisplay, mScreen), 
								 mPosX, mPosY, mWidth, mHeight, mBorderSize,
								 BlackPixel(mDisplay, mScreen), WhitePixel(mDisplay, mScreen));
	
	/* process window close event through event handler so XNextEvent does not fail */
	mDeleteWindow = XInternAtom(mDisplay, "WM_DELETE_WINDOW", True);
	XSetWMProtocols(mDisplay, mWindow, &mDeleteWindow, 1);

	XSelectInput(mDisplay, mWindow, 
		/*FocusChangeMask |*/ ExposureMask | KeyPressMask | KeyReleaseMask);

	XMapWindow(mDisplay, mWindow);

	XStoreName(mDisplay, mWindow, mName.c_str());

	// save relevant info for drawing
	mGC = XCreateGC(mDisplay, mWindow, mValuemask, &mValues);
	XSync(mDisplay, False);

	mColormap = DefaultColormap(mDisplay, mScreen);

	LOG("Window created!\n");
}

void SWindow::close(){
	XDestroyWindow(mDisplay, mEvent.xclient.window);

	XCloseDisplay(mDisplay);

	mClosed = true;
	LOG("Window closed\n");
}

/*
DRAW WINDOW (BLOCKING) --> called in a separate thread
*/
void SWindow::listener(){
	int redraw=1;
	int c;
	auto last_draw_time = std::chrono::high_resolution_clock::now();
	value ec;
	while (redraw) {
		if (!mDisplay) {
			WARNING("No display\n");
			continue;
		}

		XNextEvent(mDisplay, &mEvent);
		// XCheckWindowEvent(mDisplay, mWindow, ExposureMask, &mEvent);
		switch (mEvent.type) {
			
			// Catch events from user
			case ClientMessage:
				LOG("Client message received\n");
				// Exit event
				if (mEvent.xclient.data.l[0] == (long int)mDeleteWindow) {
					redraw=0;
					LOG("Caught window delete event\n");
				}
				break;
			
			// Draw recursively the elements in the frame
			case Expose:
				mContainer->draw(this, 0, 0);
				XFlush(mDisplay);
				break;

			case KeyPress:
				// ADD ARRAY TO DETERMINE IF A KEY IS ALREADY TRIGGERED ?
				LOG("Caught KeyPress event %d\n");
				c = mEvent.xkey.keycode;
				ec = caml_callback(*caml_named_value("makeKeyPress"), Val_int(c));
				if (mEventHandler) {
					is_Xlib = true;
					caml_callback(mEventHandler, ec);
					is_Xlib = false;
				}
				else {
					WARNING("Event handler not defined\n");
				}
				break;

			case KeyRelease:
				
				if(XPending(mDisplay)){
					XEvent nev;
					XPeekEvent(mDisplay, &nev);

			  		if (nev.type == KeyPress && nev.xkey.time == mEvent.xkey.time &&
				      nev.xkey.keycode == mEvent.xkey.keycode){
				      	LOG("Caught false KeyRelease\n");
				      	XNextEvent(mDisplay, &mEvent);
				      	break;
			  		}
				}

				LOG("Caught KeyRelease event\n");

				c = mEvent.xkey.keycode; 
				ec = caml_callback(*caml_named_value("makeKeyReleased"), Val_int(c));
				if (mEventHandler) {
					is_Xlib = true;
					caml_callback(mEventHandler, ec);
					is_Xlib = false;
				}
				else {
					WARNING("Event handler not defined\n");
				}
				break;
			default:
				WARNING("Caught unknown event\n");
				break;
		}

		// Handle action	
		mActionMutex.lock();
		if (!mSharedQueue.empty() && redraw) {
			Action a = mSharedQueue.front();
			mSharedQueue.pop();

			LOG("Received message from CAML: \n");
			a.Call();
			auto actual_time = std::chrono::high_resolution_clock::now();
			if (std::chrono::duration_cast<std::chrono::microseconds>(actual_time-last_draw_time).count() > 100000) {  	
				mContainer->draw(this, 0, 0);
				XFlush(mDisplay);
				last_draw_time = actual_time;
				WARNING("Drawing\n");
			}

			a.mResultLock->unlock();
		}
		mActionMutex.unlock();

	}
	LOG("Closing the thread\n");
	this->close();
}

void SWindow::draw(){
	if (!mThread) {
		mThread	= new thread(&SWindow::listener, this); 
	}
}
