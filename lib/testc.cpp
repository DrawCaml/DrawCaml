#define CAML_NAME_SPACE
#include "caml/mlvalues.h"
#include "caml/alloc.h"
#include "caml/memory.h"
#include "caml/fail.h"
#include "caml/callback.h"
#include "caml/custom.h"
#include "caml/intext.h"
#include "caml/intext.h"
#include "caml/threads.h"


#include <stdio.h>
#include <stdlib.h>

#include <X11/Xlib.h>
#include <unistd.h>
#include "window.h"
#include <iostream>

extern "C" value test_cpp(value n1) {
	SWindow win;
  
	win.create("test DrawCaml", 10, 10, 500, 500, 1);
  	win.draw();
	std::cout << "Test succesfull!" << std::endl;

	return (Val_int(0));
}
