external createWindowEx : string -> int -> int -> int -> int -> nativeint = "createWindow_cpp"
external drawWindowEx : nativeint -> unit = "draw_cpp"
external sendMessage : nativeint -> int -> unit = "sendMessage_cpp"
external waitForCloseEx : nativeint -> unit = "waitForClose_cpp"
external setWindowContainerEx : nativeint -> nativeint -> unit = "setWindowContainer_cpp"

external createContainerEx : int -> int -> int -> nativeint = "createContainer_cpp"
external setPosEx : nativeint -> int -> int -> unit = "setPos_cpp"
external setSizeEx : nativeint -> int -> int -> unit = "setSize_cpp"
external addElemEx : nativeint -> nativeint -> int -> int -> unit = "addElem_cpp"
external setBgColorEx : nativeint -> string -> unit = "setBgColor_cpp"

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
		method getSize () =
			size
		method getPos () =
			pos
		method setSize t =
			size <- t
		method setPos t =
			pos <- t
	end

(* graphical element that can manage others *)
class dcontainer ?(layout = FloatLayout) ?(dim = (1,1)) () =
	object
		inherit delement ()
		val mutable delement_list = ([] : delement list)
		val ptr = createContainerEx (layout_enum layout) (fst dim) (snd dim)
		(* method addElem : 'a. (#delement as 'a) -> int*int -> unit = fun elem pos_elem ->
			let elem_coerce = (elem :> delement) in
			elem_coerce#setPos pos_elem;
			delement_list <- elem_coerce::delement_list *)
		method setBgColor (col : string) =
			(setBgColorEx ptr col)
		method getPtr () =
			ptr
	end

(* class for the window *)
class dwindow ?(title = "DrawCaml Window") ?(pos = (10,10)) ?(size = (100,100)) () =
	object
		val mutable main_container = new dcontainer ()
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
				failwith("Pointeur zéro pas cool")
		method waitForClose () =
			waitForCloseEx(ptr)
		initializer setWindowContainerEx ptr (main_container#getPtr ())
	end