(* should return a DWindow instance *)
let window = new DrawCaml.dwindow ~title:"Test Window" ~pos:(50,50) ~size:(500,500) ();;

let win_container = window#getContainer ();;

win_container#setBgColor("yellow");;

let grid_container = new DrawCaml.dcontainer ~layout:GridLayout ~dim:(5,5) ();;

win_container#add grid_container ~pos:(100,100) ();;

grid_container#setBgColor("green");;

let container1 = new DrawCaml.dcontainer ();;
let container2 = new DrawCaml.dcontainer ();;
let container3 = new DrawCaml.dcontainer ();;
let container4 = new DrawCaml.dcontainer ();;

container1#setBgColor("red");;
container2#setBgColor("blue");;
container3#setBgColor("orange");;
container4#setBgColor("purple");;

grid_container#add container2 ();;
grid_container#add container1 ();;
grid_container#add container3 ~pos:(3,3) ();;
grid_container#add container4 ~pos:(1,2) ();;


(* test label *)
let testLabel = new DrawCaml.dlabel ~text:"testlabel" ();;
testLabel#setColor "blue";;
(* testLabel#setFont "fixed";; *)
grid_container#add testLabel ~pos:(0,4) ();;

print_int (fst (container4#getPos ()));;
print_newline ();;
print_int (snd (container4#getPos ()));;
print_newline ();;
print_int (fst (container4#getSize ()));;
print_newline ();;
print_int (snd (container4#getSize ()));;
print_newline ();;

let f e =
	match e with
	|DrawCaml.KeyPressed(_) -> 	print_string("press");
								print_newline ();
								testLabel#setColor "white";
								grid_container#add container1 ~pos:(2,2) ()
	|DrawCaml.KeyReleased(_) -> print_string("released");
								print_newline ();
								testLabel#setColor "purple";
								grid_container#add container1 ~pos:(1,0) ()
	|DrawCaml.MousePressed(x,y,b) -> print_string("Mouse pressed: x=");
								print_int(x); print_string(" y="); print_int(y);
								print_string(" b="); print_int(b); print_newline()
	|DrawCaml.MouseReleased(x,y,b) -> print_string("Mouse released: x=");
								print_int(x); print_string(" y="); print_int(y);
								print_string(" b="); print_int(b); print_newline()
;;
window#setEventHandler f;;

(* keeps the window open *)
window#waitForClose ();;

(* while window#notClosed () do 
	Unix.sleep 1;
	
	(*grid_container#add container1 ();
	Unix.sleep 1;

	grid_container#remove container1;
	grid_container#add container1 ~pos:(2,2) ();*)
done;; *)
