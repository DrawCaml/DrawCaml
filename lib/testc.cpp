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
#include <pthread.h>

// void *thread_1(void *arg) {
// 	printf("Nous sommes dans le thread.\n");
// 	// Arrêt propre du thread
// 	pthread_exit(EXIT_SUCCESS);
// }
SWindow win;

extern "C" value createWindow_cpp(value n1) {
	// pthread_t thread1;
	// printf("Avant la création du thread.\n");
	// Création du thread
	// pthread_create(&thread1, NULL, thread_1, NULL);
	// pthread_join(thread1, NULL);
	// printf("Après la création du thread.\n");

  	
	win.create("test DrawCaml", 10, 10, 500, 500, 1);
  	win.draw();

	std::cout << "Test succesfull!" << std::endl;

	return (Val_int(0));
}

extern "C" value wait_cpp(value n1){
	if(&win != NULL)
	while(!win.closed){}
	std::cout << "Window Closed" << std::endl;
	return (Val_int(0));
}