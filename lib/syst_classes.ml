(* open DrawCaml *)

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
		val mutable ptr = 0
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
class dcontainer ?(layout = FloatLayout) =
	object
		inherit delement ()
		val mutable delement_list = ([] : delement list)
		val ptr = createContainerEx(layout_enum layout)
		method addElem : 'a. (#delement as 'a) -> int*int -> unit = fun elem pos_elem ->
			let elem_coerce = (elem :> delement) in
			elem_coerce#setPos pos_elem;
			element_list <- elem_coerce::element_list
		method setBgColor : int*int*int -> unit = fun (valr,valg,valb) ->
			setBgColorEx(ptr,valr,valg,valb)
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
			main_container <- ctr
		method draw () =
			if ptr then draw_elt_cpp(ptr) else failwith("Pointeur zéro pas cool")
	end
