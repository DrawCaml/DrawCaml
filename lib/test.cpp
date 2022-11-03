#include <X11/Xlib.h>
#include <unistd.h>
#include "window.h"
#include "utils.h"
#include "action.h"
#include <iostream>

// TO USE THIS TEST: (from the DrawCaml/lib directory)
// g++ test.cpp window.cpp container.cpp action.cpp -lpthread -lX11 && ./a.out

int main(){
	SWindow* win = new SWindow("test", 10, 10, 500, 500, 1);

  	// tests
	win->mContainer->setBgColor("yellow");
	SContainer* cont2 = new SContainer(SLayout::GridLayout, win, 2, 2);
	cont2->setBgColor("green");
	win->mContainer->addElem(cont2, 50, 50);
	
	SContainer* cont3 = new SContainer(SLayout::FloatLayout, win);
	cont3->setBgColor("red");
	SContainer* cont4 = new SContainer(SLayout::FloatLayout, win);
	cont4->setBgColor("blue");
	SContainer* cont5 = new SContainer(SLayout::FloatLayout, win);
	cont5->setBgColor("orange");
	// SContainer* cont6 = new SContainer(SLayout::FloatLayout, win);

	cont2->addElem(cont3);
	cont2->addElem(cont4);
	cont2->addElem(cont5, 1, 1);
	
  	win->draw();

  	LOG("Waiting for window '" + win->mName + "' to close\n");
	while(!win->mClosed) {
		continue;
	}
	LOG("Window Closed\n");
}