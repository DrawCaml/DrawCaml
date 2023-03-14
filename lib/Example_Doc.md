# Tutorial

# Dependencies

DrawCaml is an OCaml library on top of C++ X11. It is compiled using Opam. 
The dependencies are:

- [x11 library](https://www.x.org)
- [opam](https://opam.ocaml.org/doc/Install.html)
- dune and dune-configurator (`opam install dune dune-configurator`)

# Installing the library and testing it

Once you got all dependencies, you can clone this repository:

`git clone https://github.com/Drawcaml/drawcaml.git`

You can then run an example code with:

`dune exec bin/snake.exe` for example.

If everything worked, you should see a window getting opened.

# Simple example

Like many graphical libraries, DrawCaml uses containers to manage graphical elements. To see how to use it, there is the full documentation available [here](../../ocaml/DrawCaml.html) .

Here is a very short example, that simply creates a window of size 500 by 500, and puts it's background color to yellow.

```ocaml
let window = new DrawCaml.dwindow ~title:"Test Window" ~pos:(50,50) ~size:(500,500) ();;
let win_container = window#getContainer ();;
win_container#setBgColor("yellow");;

window#waitForClose ();;
```

![example1](../../example1.png)

Here we see that the window has a main container, that itself can have childs. Changing the color of this container changes the color of the whole window. The call to `window#waitForClose()` simply tells the library that we want to still have the window open after the program ended, until the user decides to close it.

We can now try to add some simple containers inside of the window:

```ocaml
let window = new DrawCaml.dwindow ~title:"Test Window" ~pos:(50,50) ~size:(500,500) ();;
let win_container = window#getContainer ();;

let grid_container = new DrawCaml.dcontainer ~layout:GridLayout ~dim:(5,5) ();;
win_container#add grid_container ~pos:(100,100) ();;

let container = new DrawCaml.dcontainer ();;

container#setBgColor("red");;

grid_container#add container ~pos:(1,2) ();;
window#waitForClose ();;
```

![example2](../../example2.png)

Here, we used the same base code as the first example, but we added a grid container as child of the main container. A grid container has a simple grid structure, where we can put other containers. To test this, we create a new container with `new DrawCaml.dcontainer` and add it to the grid container.

# Event handler

DrawCaml adds the possibility to setup an event handler, to handle the keyboard and mouse events from the user.

```ocaml
let window = new DrawCaml.dwindow ~title:"Test Window" ~pos:(50,50) ~size:(500,500) ();;
let event_handler event =
    match event with
        | DrawCaml.KeyPressed(Right)   -> Printf.printf "Right key got pressed\n%!";
        | DrawCaml.KeyReleased(_)      -> Printf.printf "A key was released\n%!";
        | DrawCaml.MousePressed(x,y,_) -> Printf.printf "The mouse was pressed at position %d %d\n%!" x y;
        | _ -> ();
;;
window#setEventHandler event_handler;;
 
 
while window#notClosed () do 
    Unix.sleep 1;
    Printf.printf "Window is still open\n%!";
done;;
```

The event handler takes an object of type `event` and can match it to perform actions accordingly. Please note that because the event handler relies on multithreading, you have to be careful about the [garbage collector](https://ocaml.org/docs/garbage-collection). Performing some simple actions shouldn't cause any issues.

Note also the different ways of telling to the program to keep the window open: using `window#notClosed ()`, we can check whether the user closed the window or not, and use it in a while condition.

For further code examples, see the folder `/bin/` from the repository, and refer to the DrawCaml documentation [here](../../ocaml/DrawCaml.html).
