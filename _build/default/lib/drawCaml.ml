(*events*)
type key =
| Letter of char
| Left
| Up 
| Right
| Down
| Space
;;

type event =
|KeyPressed of key
|KeyReleased of key
;;

(* Element methods *)
external getPosEx : nativeint -> int*int = "getPos_cpp"
external getSizeEx : nativeint -> int*int = "getSize_cpp"
external setSizeEx : nativeint -> int -> int -> unit = "setSize_cpp"

(* label methods *)
external createLabelEx : string -> int -> string -> nativeint = "createLabel_cpp"
external setTextEx : nativeint -> string -> unit = "setText_cpp"

(* Conatiner methods *)
external createContainerEx : int -> int -> int -> nativeint = "createContainer_cpp"
external setBgColorEx : nativeint -> string -> unit = "setBgColor_cpp"
external addElemEx : nativeint -> nativeint -> int -> int -> unit = "addElem_cpp"
external removeElemEx : nativeint -> nativeint -> unit = "removeElem_cpp"

(* Window methods *)
external createWindowEx : string -> int -> int -> int -> int -> nativeint = "createWindow_cpp"
external drawWindowEx : nativeint -> unit = "draw_cpp"
external waitForCloseEx : nativeint -> unit = "waitForClose_cpp"
external winNotClosedEx : nativeint -> bool = "winNotClosed_cpp"
external setWindowContainerEx : nativeint -> nativeint -> unit = "setWindowContainer_cpp"
external setWindowEventHandlerEx : nativeint -> (event -> unit) -> unit = "setWindowEventHandler_cpp"

let makeKey i = 
	match i with
	| 0x20 -> Space
	| 0xff51 -> Left
	| 0xff52 -> Up
	| 0xff53 -> Right
	| 0xff54 -> Down
	| k when (k>=0x61)&&(k<=0x7a) -> Letter(Char.chr(k))
	| _ -> Space;;

let makeKeyPressed i = (* print_string("makeKeyPressed");print_newline (); *)KeyPressed(makeKey i);;
let makeKeyReleased i = (* print_string("makeKeyReleased");print_newline (); *)KeyReleased(makeKey i);;

let _ = Callback.register "makeKeyPressed" makeKeyPressed;;
let _ = Callback.register "makeKeyReleased" makeKeyReleased;;



(*let makeArrow c = print_string("construct KeyReleased");print_newline();KeyReleased(c);;
let _ = Callback.register "makeKeyReleased" makeKeyReleased;;

let makeMousePress x y = MousePress(x,y);;
let _ = Callback.register "makeMousePress" makeMousePress;;*)

(*layouts*)
type dlayout = FloatLayout | GridLayout | Other

let layout_enum = function
	| FloatLayout -> 0
	| GridLayout -> 1
	| Other -> 2

(* abstract type for elements of the window *)
class virtual delement () =
	object
		val mutable size = (-1,-1)
		val mutable pos = (-1,-1)
		val mutable ptr = 0n
		method getSize () =
			getSizeEx ptr
		method setSize (sx,sy) =
			setSizeEx ptr sx sy
		method getPos () =
			getPosEx ptr
		method getPtr () =
			ptr
	end

(* graphical element that prints text *)
class dlabel ?(text="") ?(font="helvetica") ?(fontSize=12) () =
	object
		inherit delement ()
(* 		method setColor (col : string) = 
			(setFgColorEx ptr col) *)
		method setText (txt : string) =
			(setTextEx ptr txt)
		initializer ptr <- createLabelEx text fontSize font
end

(* graphical element that prints an Image *)

(* graphical element that can manage others *)
class dcontainer ?(layout = FloatLayout) ?(dim = (1,1)) () =
	object
		inherit delement ()
		method setBgColor (col : string) =
			(setBgColorEx ptr col)
		method add : 'a. (#delement as 'a) -> ?pos:int*int -> unit -> unit= fun elt ?(pos=(-1,-1)) ()->
			let e = (elt :> delement) in
			(addElemEx ptr (e#getPtr()) (fst pos) (snd pos))
		method remove : 'a. (#delement as 'a) -> unit = fun elt ->
			let e = (elt :> delement) in
			(removeElemEx ptr (e#getPtr()))
		initializer ptr <- createContainerEx (layout_enum layout) (fst dim) (snd dim)
	end

(* class for the window *)
class dwindow ?(title = "DrawCaml Window") ?(pos = (10,10)) ?(size = (100,100)) () =
	object
		val mutable main_container = new dcontainer ()
		val mutable event_handler = (fun _ -> ())
		val ptr = createWindowEx title (fst pos) (snd pos) (fst size) (snd size)
		method getTitle () =
			title
		method getSize () =
			size
		method getPos () =
			pos
		method getContainer () =
			main_container
		method setContainer ctr =
			begin
			(main_container <- ctr);
			(setWindowContainerEx ptr (ctr#getPtr ()))
			end
		method draw () =
			if ptr<>0n then 
				drawWindowEx(ptr) 
			else 
				failwith("OCaml: NullPointerException")
		method notClosed () =
			winNotClosedEx ptr
		method waitForClose () =
			waitForCloseEx(ptr)
		method setEventHandler f =
			begin
				(event_handler <- f);
				(setWindowEventHandlerEx ptr f)
			end
		initializer setWindowContainerEx ptr (main_container#getPtr ())
	end
