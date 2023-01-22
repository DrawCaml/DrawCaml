(* should return a DWindow instance *)
let window = new DrawCaml.dwindow ~title:"Test Window" ~pos:(50,50) ~size:(400,400) ();;

let grid_container = new DrawCaml.dcontainer ~layout:GridLayout ~dim:(5,5) ();;

window#setContainer(grid_container);;

grid_container#setBgColor("green");;

let container1 = new DrawCaml.dcontainer ();;
let container2 = new DrawCaml.dcontainer ();;
let container3 = new DrawCaml.dcontainer ();;
let container4 = new DrawCaml.dcontainer ();;

container1#setBgColor("red");;
container2#setBgColor("blue");;
container3#setBgColor("orange");;
container4#setBgColor("purple");;

grid_container#addElem container1 ();;
grid_container#addElem container2 ();;
grid_container#addElem container3 ~pos:(3,3) ();;
grid_container#addElem container4 ~pos:(1,2) ();;

(* keeps the window open *)
window#waitForClose ();;