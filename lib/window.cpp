#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <thread>
#include <mutex>

#include <X11/Xlib.h>
#include "window.h"
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

  	XMapWindow(mDisplay, mWindow);

  	XStoreName(mDisplay, mWindow, mName.c_str());
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
	while (redraw) {
		if (!mDisplay) {
			WARNING("No display\n");
			continue;
		}
		XNextEvent(mDisplay, &mEvent);
		switch (mEvent.type) {
			case ClientMessage:
				LOG("Client message received\n");
				// Exit event
				if (mEvent.xclient.data.l[0] == (long int)mDeleteWindow) {
					redraw=0;
					LOG("Caught window delete event\n");
				}
				break;
			default:
				WARNING("Caught unknown event\n");
				break;
			}

		mActionMutex.lock();
		if (!mSharedQueue.empty()) {
			Action a = mSharedQueue.front();
			mSharedQueue.pop();

			// Handle action	
			LOG("Received message from CAML: \n");
			a.Call();

			a.mResultLock->unlock();
		}
		mActionMutex.unlock();
	}
	LOG("Closing the thread\n");
  	this->close();
}

void SWindow::draw(){
	if (!mThread) {
  		mThread  = new thread(&SWindow::listener, this); 
	}
}
