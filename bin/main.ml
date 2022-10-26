(* should return a DWindow instance *)
let window = DrawCaml.createWindow("Window1");;

for i = 0 to 4 do
	DrawCaml.sendMessage window i;
	Unix.sleep(1);
done;;

(* keeps the window open *)
DrawCaml.waitForClose(window);;
