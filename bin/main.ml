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

(* grid_container#add container1 ();; *)
grid_container#add container2 ();;
grid_container#add container3 ~pos:(3,3) ();;
grid_container#add container4 ~pos:(1,2) ();;


(* test label *)
let testLabel = new DrawCaml.dlabel ~text:"testlabel" ();;
grid_container#add testLabel ~pos:(0,4) ();;

(* marche pas sans le Unix.sleep *)
Unix.sleepf 0.1;;
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
	|DrawCaml.KeyPress(_) -> 	print_string("press\n");
								grid_container#remove container1;
								grid_container#add container1 ~pos:(2,2) ();
	|DrawCaml.KeyReleased(_) -> print_string("released\n");
								grid_container#remove container1;
								grid_container#add container1 ~pos:(1,0) ();
	|_ -> ();;

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