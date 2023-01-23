#pragma once

#include <string>
#include <X11/Xlib.h>
#include <thread>
#include <mutex>
#include <queue>
#include <vector>

#include "element.h"
#include "container.h"
#include "action.h"

using namespace std;

class SWindow {
	public:
		string mName;
		int mPosX, mPosY;
		int mWidth, mHeight, mBorderSize;

		thread* mThread;
		mutex mActionMutex;
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


		SWindow(string name, int posX, int posY, int width, int height, int borderSize);

		void close();
		void draw();
		void listener();

};
