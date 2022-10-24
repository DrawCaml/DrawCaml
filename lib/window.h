#pragma once

#include <string>
#include <X11/Xlib.h>
#include <thread>
#include <mutex>
#include <queue>

using namespace std;

class SWindow {
	public:
		string mName;
		int mPosX, mPosY;
		int mWidth, mHeight, mBorderSize;

		thread* mThread;
		mutex mActionMutex;
		queue<int> mSharedQueue;
		bool mClosed;

		/*
	 	XLib parameters
		*/
		Window mWindow;
		Display *mDisplay;
		int mScreen;
	
		XEvent mEvent;
		Atom mDeleteWindow; // used for clean exit


		SWindow(string name, int posX, int posY, int width, int height, int borderSize);

		void close();
		void draw();
		void listener();

};
