(* should return a DWindow instance *)
let window = DrawCaml.createWindow("Window1");;
let window2 = DrawCaml.createWindow("Window2");;

for i = 0 to 4 do
	DrawCaml.sendMessage window i;
	DrawCaml.sendMessage window2 i;
	Unix.sleep(1);
done;;

(* keeps the window open *)
DrawCaml.waitForClose(window);;
DrawCaml.waitForClose(window2);;
