(* should return a DWindow instance *)
let window = new DrawCaml.dwindow ~title:"Test Window" ~pos:(50,50) ~size:(500,500) ();;

let win_container = window#getContainer ();;

win_container#setBgColor("yellow");;

let grid_container = new DrawCaml.dcontainer ~layout:GridLayout ~dim:(5,5) ();;

win_container#add grid_container ~pos:(100,100) ();;

grid_container#setBgColor("green");;

(* BUG: ce Unix.sleep est nécéssaire *)
Unix.sleep 1;;
(* car sinon les actions ci-dessous sont éxécutées avant la ligne 10,*)
(* car la l10 est ajoutée à la queue donc son éxécution est *légèrement* *)
(* retardée. *)
(* SOL: garder pour chaque élément une file d'action à faire effectuer *)
(* à une window dès lors que ce pointeur sera non NULL *)

let container1 = new DrawCaml.dcontainer ();;
let container2 = new DrawCaml.dcontainer ();;
let container3 = new DrawCaml.dcontainer ();;
let container4 = new DrawCaml.dcontainer ();;

container1#setBgColor("red");;
container2#setBgColor("blue");;
container3#setBgColor("orange");;
container4#setBgColor("purple");;

grid_container#add container1 ();;
grid_container#add container2 ();;
grid_container#add container3 ~pos:(3,3) ();;
grid_container#add container4 ~pos:(1,2) ();;


(* marche pas sans le Unix.sleep *)
Unix.sleep 1;;
print_int (fst (container4#getPos ()));;
print_newline ();;
print_int (snd (container4#getPos ()));;
print_newline ();;
print_int (fst (container4#getSize ()));;
print_newline ();;
print_int (snd (container4#getSize ()));;
print_newline ();;

(* keeps the window open *)
(* window#waitForClose ();; *)
while window#notClosed () do 
	Unix.sleep 1
done;;