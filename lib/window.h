#pragma once

#define CAML_NAME_SPACE
#include "caml/mlvalues.h"
#include "caml/alloc.h"
#include "caml/memory.h"
#include "caml/fail.h"
#include "caml/callback.h"
#include "caml/custom.h"
#include "caml/intext.h"
#include "caml/threads.h"

#include <string>
#include <X11/Xlib.h>
#include <X11/extensions/Xdbe.h>
#include <thread>
#include <mutex>
#include <queue>
#include <vector>

#include "element.h"
#include "container.h"
#include "action.h"

using namespace std;

/**
 * Class for printing the window and its graphical elements by calling X11 library.
 * */
class SWindow {
	public:
		//window parameters
		string mName;
		int mPosX, mPosY;
		int mWidth, mHeight, mBorderSize;

		//attributes for user actions handling
		thread* mThread;
		mutex mActionMutex;
		/**
		 * Queue of actions to perform on the window.
		 * */
		queue<Action> mSharedQueue;

		bool mClosed = false;

		SContainer* mContainer;

		/*
	 	XLib parameters
		*/
		Window mWindow;
		Display *mDisplay;
		int mScreen;
	
		XEvent mEvent;
		Atom mDeleteWindow; // used for clean exit

		unsigned long mValuemask = 0;
		XGCValues mValues;	

		GC mGC;
		Colormap mColormap;

		/**
		 * Back buffer used for clean redraw operations using XDoubleBufferExtension.
		 * */
		XdbeBackBuffer mBackBuffer;

		// event handling attributes
		value mEventHandler;
		bool is_Xlib = false;

		SWindow(string name, int posX, int posY, int width, int height, int borderSize);

		/**
		 * Clean closure of X window.
		 * */
		void close();
		/**
		 * Create the "Xlib" thread that will listen to events and draw the window.
		 * */
		void draw();
		/**
		 * Loop for listening X11 events.
		 * */
		void listener();
		value keyEventToCaml(int keycode, bool is_pressed);

};
