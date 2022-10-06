#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <pthread.h>
// #include <thread>

#include <X11/Xlib.h>
#include "window.h"

SWindow::SWindow(){
  // constructor
  std::cout << "Window created !" << std::endl;

  XInitThreads();
}

/*
CREATE WINDOW
*/
void SWindow::create(std::string Uname, int Ux, int Uy, int Uwidth, int Uheight, int Uborder){

  name = Uname;
  pos_x = Ux;
  pos_y = Uy;
  width = Uwidth;
  height = Uheight;
  border = Uborder;

  // Open the connection with the X server
  display = XOpenDisplay(NULL);
  if(display == NULL){
	// fprintf(stderr, "Cannot open display !\n");
	std::cout << "Cannot open display !" << std::endl;
	exit(1);
  }

  // choose the screen
  screen = DefaultScreen(display);

  // create window
  window = XCreateSimpleWindow(display, RootWindow(display, screen), 
	  // window parameters
	  pos_x, pos_y, width, height, border,
	  BlackPixel(display, screen), WhitePixel(display, screen));
  
  /* process window close event through event handler so XNextEvent does not fail */
	wmDeleteWindow = XInternAtom(display, "WM_DELETE_WINDOW", True);
	XSetWMProtocols(display, window, &wmDeleteWindow, 1);

	// map the window
  XMapWindow(display, window);

  // Set the name of the window
  XStoreName(display, window, name.c_str());
}

/*
CLOSE WINDOW
*/
void SWindow::close(){

  /* destroy window */
	XDestroyWindow(display, event.xclient.window);


	// std::cout << (display == NULL) << std::endl;
	/* close connection to server */
	XCloseDisplay(display);

	closed = true;
}

/*
DRAW WINDOW (BLOCKING) --> called in a separate thread
*/
void* iDraw(void *arg){
	SWindow* win = (SWindow*) arg;

  int redraw=1;
  /* draw the window */
  while (redraw) {
  	// XLockDisplay(display);
		XNextEvent(win->display, &(win->event));
		switch (win->event.type) {
	  	case ClientMessage:
			// Exit event
			if (win->event.xclient.data.l[0] == (long int)win->wmDeleteWindow)
		  	redraw=0;
			break;
		}
		// XUnlockDisplay(display);
  }
  win->close();
  pthread_exit(EXIT_SUCCESS);
}

// create the drawing thread
void SWindow::draw(){
	if(&_thread != NULL){
		pthread_create(&_thread, NULL, iDraw, (void*)this);
	} else {
		std::cout << "thread already created" << std::endl;
	}
}
