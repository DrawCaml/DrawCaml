Random.self_init ();;
(* should return a DWindow instance *)
let window = new DrawCaml.dwindow ~title:"SNAKE" ~pos:(50,50) ~size:(800,800) ();;

let win_container = window#getContainer ();;

(* grid sidelength *)
let gsl = 20;;

win_container#setBgColor("black");;

let grid_container = new DrawCaml.dcontainer ~layout:GridLayout ~dim:(gsl, gsl) ();;

win_container#add grid_container ~pos:(0,0) ();;

grid_container#setBgColor("black");;

type cell = Empty | Snake | Food;;
type direction = North | South | East | West;;

type game_state = {
    snake: (int*int) Queue.t;
    mutable dir: direction;
    grid: cell array array;
    mutable head: int*int;
    mutable lost: bool;
    mutable score: int;
};;

let gtest = new DrawCaml.dcontainer ~layout:FloatLayout ();;

let grid = Array.make_matrix gsl gsl Empty;;
let grid_containers = Array.init gsl (fun _ -> Array.init gsl (fun _ -> new DrawCaml.dcontainer ()));;
for i=0 to gsl-1 do
    for j=0 to gsl-1 do
            grid_container#add grid_containers.(i).(j) ~pos:(i,j) ();
    done;
done;;


gtest#setBgColor "red";;
gtest#setSize (400,400);;

let matrix_iteri m f = Array.iteri (fun i l -> Array.iteri (fun j c -> f i j c) l) m;;

let draw state containers =
    matrix_iteri state.grid (fun i j c ->
        containers.(i).(j)#setBgColor (
            match c with
            | Empty -> "black";
            | Snake -> "white";
            | Food  -> "green";
        )
    );;

let snake = Queue.create ();;
Queue.push (gsl/2 - 2, gsl/2) snake;;
Queue.push (gsl/2 - 1, gsl/2) snake;;
Queue.push (gsl/2, gsl/2) snake;;
Queue.iter (fun (i,j) -> grid.(i).(j) <- Snake) snake;;

let game = {
    snake = snake;
    grid = grid;
    dir = North;
    head = (gsl/2, gsl/2);
    lost = false;
    score = 0;
};;

draw game grid_containers;;

let is_cell_fail state (i, j) =
    (
        (i >= gsl || j >= gsl)
        || (i < 0 || j < 0)
        || (state.grid.(i).(j) = Snake)
    );;

let new_coord (i, j) = function
    | East -> (i + 1, j);
    | West -> (i - 1, j);
    | North -> (i, j - 1);
    | South -> (i, j + 1);;

let rec add_food state =
    let i = Random.int gsl in
    let j = Random.int gsl in
    if state.grid.(i).(j) <> Empty then
        add_food state
    else state.grid.(i).(j) <- Food;;

add_food game;;

let snake_step state =
    let new_head = new_coord state.head state.dir in
    if is_cell_fail state new_head then (
        state.lost <- true;
        print_string("EPIC FAIL OMG\n");
    ) else (
        state.head <- new_head;
        let (ni, nj) = new_head in if state.grid.(ni).(nj) <> Food then
            let (i,j) = Queue.pop state.snake in state.grid.(i).(j) <- Empty;
        else (
            state.grid.(ni).(nj) <- Snake;
            add_food state;
            state.score <- state.score + 1;
            Printf.printf "Score: %d" state.score;
            print_newline();
        );
        Queue.push new_head state.snake;
        let (i,j) = new_head in state.grid.(i).(j) <- Snake;
    );;

let change_dir newdir state =
    match (newdir, state.dir) with
    | (East, West)
    | (West, East)
    | (North, South)
    | (South, North) -> ();
    | _ -> state.dir <- newdir;;


let f e =
	match e with
    |DrawCaml.KeyPressed(Right) -> change_dir East  game;
    |DrawCaml.KeyPressed(Down) -> change_dir South game;
    |DrawCaml.KeyPressed(Left) -> change_dir West  game;
    |DrawCaml.KeyPressed(Up) -> change_dir North game;
	|_ -> ();;

window#setEventHandler f;;


let lastTime = ref (Unix.gettimeofday ());;
let actualTime = ref 0.;;

while window#notClosed () do
    actualTime := Unix.gettimeofday ();
    
    (* use this condition instead of Unix.sleep *)
    if !actualTime -. !lastTime > 0.10 then begin
        
        if game.lost then begin
            win_container#add gtest ~pos:(200,200) ()
        end else begin
            snake_step game;
            draw game grid_containers;
        end;
        lastTime := !actualTime;
    
    end
    (* LAGGY: Unix.sleepf 0.07; *)
done;;
