#ifndef WINDOW_H
#define WINDOW_H

#include <string>
#include <X11/Xlib.h>

class SWindow {

// XLib parameters
private:
	Display *display;
	Window window;
	XEvent event;
	int screen;

	Atom wmDeleteWindow; // used for clean exit

// configuration variables and functions
public:
	std::string name;
	int pos_x, pos_y;
	int width, height, border;

	SWindow();

	void create(std::string name, int win_x, int win_y, int win_width, int win_height, int win_border);
	
	void close();
	
	void draw();

};

#endif