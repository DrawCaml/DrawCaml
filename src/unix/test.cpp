#include <iostream>

#include "window.h"

int main(int argc, char const *argv[]){
	
	std::cout << "Hello" << std::endl;

	SWindow win; // = new SWindow();
	
	win.create("test DrawCaml", 10, 10, 500, 500, 1);
	win.draw();
	// win.close(); //<-- cause une segfault

	return 0;
}