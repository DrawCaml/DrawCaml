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
| KeyPressed of key
| KeyReleased of key
;;


(**/**)
(* Element methods *)
external getPosEx : nativeint -> int*int = "getPos_cpp"
external getSizeEx : nativeint -> int*int = "getSize_cpp"
external setSizeEx : nativeint -> int -> int -> unit = "setSize_cpp"

(* label methods *)
external createLabelEx : string -> int -> string -> nativeint = "createLabel_cpp"
external setTextEx : nativeint -> string -> unit = "setText_cpp"
external setLabelColorEx : nativeint -> string -> unit = "setLabelColor_cpp"
external setLabelFontEx : nativeint -> string -> unit = "setLabelFont_cpp"

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

let makeKeyPressed i = KeyPressed(makeKey i);;
let makeKeyReleased i = KeyReleased(makeKey i);;

let _ = Callback.register "makeKeyPressed" makeKeyPressed;;
let _ = Callback.register "makeKeyReleased" makeKeyReleased;;

(*let makeMousePress x y = MousePress(x,y);;*)
(**/**)


(*
let makeMousePress x y = MousePress(x,y);;
let _ = Callback.register "makeMousePress" makeMousePress;;*)

(** Different types of layout. In the float layout, you give relative pixel positions,
in the grid layout, you give indexes *)
type dlayout = FloatLayout | GridLayout | Other

(**/**)
let layout_enum = function
	| FloatLayout -> 0
	| GridLayout -> 1
	| Other -> 2
(**/**)

(** Abstract type for the elements of the window *)
class virtual delement () =
	object
        (**/**)
		val mutable size = (-1,-1)
		val mutable pos = (-1,-1)
		val mutable ptr = 0n
        (**/**)
        (** Gets the size of the element *)
		method getSize () =
			getSizeEx ptr

        (** Sets the size of the element *)
		method setSize (sx,sy) =
			setSizeEx ptr sx sy

        (** Get the relative position of the element *)
		method getPos () =
			getPosEx ptr

        (**/**)
		method getPtr () =
			ptr
        (**/**)
	end

(** Graphical element that prints text *)
class dlabel ?(text="") ?(font="*helvetica*-r-*-20-*") ?(fontSize=12) () =
	object
		inherit delement ()

        (** Sets the color of the label *)
		method setColor (col : string) = 
			(setLabelColorEx ptr col)

		(** Specify the font of the label using X logical font format (use ```xlsfonts``` to see available fonts) *)
		method setFont (ft : string) =
			(setLabelFontEx ptr ft)

        (** Sets the text of the label *)
		method setText (txt : string) =
			(setTextEx ptr txt)
		initializer ptr <- createLabelEx text fontSize font
end

(* graphical element that prints an Image *)

(** Base element, that simply contains other elements *)
class dcontainer ?(layout = FloatLayout) ?(dim = (1,1)) () =
	object
		inherit delement ()

        (** Set the color of the background of the countainer *)
		method setBgColor (col : string) =
			(setBgColorEx ptr col)

        (** Add an element in the container, at a given position *)
		method add : 'a. (#delement as 'a) -> ?pos:int*int -> unit -> unit= fun elt ?(pos=(-1,-1)) ()->
			let e = (elt :> delement) in
			(addElemEx ptr (e#getPtr()) (fst pos) (snd pos))

        (** Removes a specified element from the container *)
		method remove : 'a. (#delement as 'a) -> unit = fun elt ->
			let e = (elt :> delement) in
			(removeElemEx ptr (e#getPtr()))
		initializer ptr <- createContainerEx (layout_enum layout) (fst dim) (snd dim)
	end

(** Main window class *)
class dwindow ?(title = "DrawCaml Window") ?(pos = (10,10)) ?(size = (100,100)) () =
	object
        (**/**)
		val mutable main_container = new dcontainer ()
		val mutable event_handler : event -> unit = (fun _ -> ())
		val ptr = createWindowEx title (fst pos) (snd pos) (fst size) (snd size)
        (**/**)

        (** Get the titl of the window *)
		method getTitle () =
			title

        (** Set the title of the window *)
		method getSize () =
			size

        (** Set the position of the window *)
		method getPos () =
			pos

        (** Get the main container of the window *)
		method getContainer () =
			main_container

        (** Set the main container of the window *)
		method setContainer ctr =
			begin
			(main_container <- ctr);
			(setWindowContainerEx ptr (ctr#getPtr ()))
			end

        (** Recursively redraw the childs of the window *)
		method draw () =
			if ptr<>0n then 
				drawWindowEx(ptr) 
			else 
				failwith("OCaml: NullPointerException")

        (** Checks if the windows is closed *)
		method notClosed () =
			winNotClosedEx ptr

        (** Hangs the thread while the windows hasn't been closed *)
		method waitForClose () =
			waitForCloseEx(ptr)

        (** Binds an event handler for the user keyboard interraction *)
		method setEventHandler f =
			begin
				(event_handler <- f);
				(setWindowEventHandlerEx ptr f)
			end
		initializer setWindowContainerEx ptr (main_container#getPtr ())
	end
