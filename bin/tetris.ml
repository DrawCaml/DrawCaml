Random.self_init ();;

let wait x =
	let actualTime = Unix.gettimeofday () in
	while Unix.gettimeofday () -. actualTime < x do
		()
	done;;

let window = new DrawCaml.dwindow ~title:"TETRIS" ~pos:(50,50) ~size:(1000,1000) ();;
let win_container = window#getContainer ();;
win_container#setBgColor("beige");;

let game_container = new DrawCaml.dcontainer ~layout:FloatLayout ();;
game_container#setSize (400,800);;
win_container#add game_container ~pos:(200,100) ();;

let grid_container = new DrawCaml.dcontainer ~layout:GridLayout ~dim:(10,20) ();;
grid_container#setBgColor "gray22";;
game_container#add grid_container ~pos:(0,0) ();;

let next_container = new DrawCaml.dcontainer ~layout:FloatLayout ();;
next_container#setSize (160,160);;
win_container#add next_container ~pos:(700,200) ()

let next_grid_container = new DrawCaml.dcontainer ~layout:GridLayout ~dim:(4,4) ();;
next_grid_container#setBgColor "beige";;
next_container#add next_grid_container ~pos:(0,0) ();;

let nextLabel = new DrawCaml.dlabel ~text:"aaa" ~font:"Roboto" ();;
grid_container#add nextLabel ~pos:(1,1) ();;

type block = I | J | L | O | S | T | Z;;
type square = Empty | Current of block | Dead of block;;
type action = Rem | Add | Kill;;
type shift = Left | Right | None

let get_random_block () =
	match Random.int 7 with
		| 0 -> I | 1 -> J | 2 -> L | 3 -> O | 4 -> S | 5 -> T | 6 -> Z;;

let grid = Array.make_matrix 10 20 Empty;;

let next_grid = Array.make_matrix 4 4 Empty;;

let get_base_coords = function
	| I -> [|(-2,0);(-1,0);(0,0);(1,0)|]
	| J -> [|(-1,0);(-1,1);(0,1);(1,1)|]
	| L -> [|(1,0);(-1,1);(0,1);(1,1)|]
	| O -> [|(-1,0);(0,0);(-1,1);(0,1)|]
	| S -> [|(-1,1);(0,1);(0,0);(1,0)|]
	| T -> [|(-1,1);(0,1);(1,1);(0,0)|]
	| Z -> [|(-1,0);(0,0);(0,1);(1,1)|];;

let get_color = function
	| I -> "cyan"
	| J -> "blue"
	| L -> "orange"
	| O -> "yellow"
	| S -> "green"
	| T -> "purple"
	| Z -> "red";;

let matrix_iteri m f = Array.iteri (fun i l -> Array.iteri (fun j c -> f i j c) l) m;;

let draw grid containers =
    matrix_iteri grid (fun i j c ->
        containers.(i).(j)#setBgColor (
            match c with
            | Empty -> "gray22";
            | Current b -> get_color b;
            | Dead b -> get_color b;
        )
);;

let draw_next grid containers =
    matrix_iteri grid (fun i j c ->
        containers.(i).(j)#setBgColor (
            match c with
            | Empty -> "beige";
            | Current b -> get_color b;
            | Dead b -> get_color b;
        )
);;

let get_current_coords orig block  =
	let coords = get_base_coords block in
	let (o1,o2) = orig in
	Array.map (fun (c1,c2) -> (c1+o1,c2+o2)) coords;;

let manage_block orig block action  =
	let coords = get_current_coords orig block in
	for i = 0 to 3 do
		let (x1,x2) = coords.(i) in
		grid.(x1).(x2) <- (match action with Add -> Current block | Rem -> Empty | Kill -> Dead block);
	done;
;;

let manage_next_block orig block action  =
	let coords = get_current_coords orig block in
	for i = 0 to 3 do
		let (x1,x2) = coords.(i) in
		next_grid.(x1).(x2) <- (match action with Add -> Current block | Rem -> Empty | Kill -> Dead block);
	done;
;;

let block_fallen orig block  =
	let coords = get_current_coords orig block in
	let b = ref false in
	for i = 0 to 3 do
		let (x1,x2) = coords.(i) in
		if x2+1 >= 20 then
			b := true
		else begin
			match grid.(x1).(x2+1) with
				| Dead _ -> b := true;
				| _ -> ();
		end
	done; !b;;

let full_line l =
	let b = ref true in
	for i = 0 to 9 do
		match grid.(i).(l) with
			|Dead _ -> ();
			|_ -> b := false;
	done; !b;;

let next_grid_containers = Array.init 4 (fun _ -> Array.init 4 (fun _ -> new DrawCaml.dcontainer ()));;
for i=0 to 3 do
    for j=0 to 3 do
    		next_grid_containers.(i).(j)#setBgColor "beige";
            next_grid_container#add next_grid_containers.(i).(j) ~pos:(i,j) ();
    done;
done;;

let grid_containers = Array.init 10 (fun _ -> Array.init 20 (fun _ -> new DrawCaml.dcontainer ()));;
for i=0 to 9 do
    for j=0 to 19 do
    		grid_containers.(i).(j)#setBgColor "black";
            grid_container#add grid_containers.(i).(j) ~pos:(i,j) ();
    done;
done;;

let remove_lines arr =
	let l_arr = Array.length arr in
	let buffer = Array.make_matrix 10 l_arr (Dead I) in
	for k = 0 to 1 do
		for i = 0 to 9 do
			for j = 0 to l_arr-1 do
				buffer.(i).(j) <- grid.(i).(arr.(j)) ; grid.(i).(arr.(j)) <- Empty;
			done;
		done;
		wait 0.2 ; draw grid grid_containers;
		for i = 0 to 9 do 
			for j = 0 to l_arr-1 do
				grid.(i).(arr.(j)) <- buffer.(i).(j)
			done;
		done;
		wait 0.2 ; draw grid grid_containers; 
	done;
	for i = 0 to 9 do 
		for j = 0 to l_arr-1 do
			grid.(i).(arr.(j)) <- Empty
		done;
	done;
	wait 0.2 ; draw grid grid_containers; 
	for i = 0 to 9 do
		for j = Array.fold_left max 0 arr downto l_arr do
			grid.(i).(j) <- grid.(i).(j-l_arr);
		done;
	done;
	wait 0.2 ; draw grid grid_containers;;

let fills_line orig block =
	let coords = get_current_coords orig block in
	let filled = ref [||] in
	for i = 0 to 3 do
		let (x1,x2) = coords.(i) in
		if (full_line x2 && Array.for_all (fun x -> x <> x2) !filled) then filled := Array.append !filled [|x2|];
	done;
	if !filled <> [||] then remove_lines !filled;;

let orig = ref (5,-1);;
let step (t1,t2) = (t1,t2+1);;
let shift_left (t1,t2) block =
	if Array.for_all (fun (c1,c2) -> c1 > 0 && match grid.(c1-1).(c2) with Dead _ -> false | _ -> true) (get_current_coords (t1,t2) block) then
		(t1-1,t2)
	else
		(t1,t2);;

let shift_right (t1,t2) block =
	if Array.for_all (fun (c1,c2) -> c1 < 9 && match grid.(c1+1).(c2) with Dead _ -> false | _ -> true) (get_current_coords (t1,t2) block) then
		(t1+1,t2)
	else
		(t1,t2);;

let delay = ref 0.3;;
let lastTime = ref (Unix.gettimeofday ());;
let lastTimeFrame = ref (Unix.gettimeofday ());;
let actualTime = ref 0.;;

let block = ref (get_random_block ());;

let wait_shift_right = ref false;;
let wait_shift_left = ref false;;

let next_block = ref (get_random_block ());;
let init_next_block = ref true;;

let game_step b dir =
	if snd !orig >= 0 then manage_block !orig !block Rem;
	if (b || snd !orig < 0) then orig := step (!orig);
	(match dir with None -> () | Left -> orig := shift_left !orig !block; | Right -> orig := shift_right !orig !block;);
	manage_block !orig !block Add;
	draw grid grid_containers;;

let f e =
	match e with
    |DrawCaml.KeyPressed(Right) -> wait_shift_right := true;
    |DrawCaml.KeyPressed(Down) -> delay := 0.1;
    |DrawCaml.KeyReleased(Down) -> delay := 0.3;
    |DrawCaml.KeyPressed(Left) -> wait_shift_left := true;
    (*|DrawCaml.KeyPressed(Up) -> block := rotate !block;*)
	|_ -> ();;

window#setEventHandler f;;

while window#notClosed () do
	actualTime := Unix.gettimeofday ();
	if !init_next_block then begin 
		manage_next_block (2,1) !next_block Add; draw_next next_grid next_grid_containers; init_next_block := false;
	end;
	if !wait_shift_left then begin
		(*orig := shift_left !orig !block;*)
		game_step false Left;
		draw grid grid_containers;
		wait_shift_left := false;
	end;
	if !wait_shift_right then  begin
		(* orig := shift_right !orig !block; *)
		game_step false Right;
		draw grid grid_containers;
		wait_shift_right := false;
	end;
	if !actualTime -. !lastTime > !delay then begin
		game_step true None;
		lastTime := !actualTime;
	end;
	if !actualTime -. !lastTimeFrame > 0.05 then begin
		game_step false None;
		draw grid grid_containers;
		lastTimeFrame := !actualTime;
	end;
	if block_fallen !orig !block then begin
		manage_block !orig !block Kill;
		fills_line !orig !block;
		manage_next_block (2,1) !next_block Rem;
		orig := (5,-1) ; block := !next_block; next_block := get_random_block ();
		manage_next_block (2,1) !next_block Add; draw_next next_grid next_grid_containers;
		if block_fallen !orig !block then begin while true do () done; end;
	end;
done;