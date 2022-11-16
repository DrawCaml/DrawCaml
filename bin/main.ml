(* should return a DWindow instance *)
let window = new DrawCaml.dwindow ~title:"Enrique le plus bo"
let grid_container = new DrawCaml.dcontainer ~layout:GridLayout
grid_container#setBgColor(0,255,0);
window#setContainer grid_container;
let container1 = new DrawCaml.dcontainer
let container2 = new DrawCaml.dcontainer
let container3 = new DrawCaml.dcontainer
let container4 = new DrawCaml.dcontainer
container1#setBgColor(255,0,0);
container2#setBgColor(0,0,255);
container3#setBgColor(255,128,0);
container4#setBgColor(255,0,255);

grid_container#addElem container1
grid_container#addElem container2
grid_container#addElem container3 (3,3)
grid_container#addElem container4 (1,2)

(* keeps the window open *)
DrawCaml.waitForClose(window);;
