(* should return a DWindow instance *)
DrawCaml.createWindow();;

for i = 0 to 10 do
	DrawCaml.sendMessage(i);
	Unix.sleep(1);
done;;

(* keeps the window open *)
DrawCaml.waitForClose();;
