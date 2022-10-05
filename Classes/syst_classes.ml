class virtual element () =
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
		method virtual draw : unit -> unit
	end


class container () =
	object
		inherit element ()
		val mutable element_list = ([] : element list)
		method draw () =
			List.iter (fun x -> x#draw ()) element_list
		method addElem elem pos_elem size_elem =
			elem#setPos pos_elem;
			elem#setSize size_elem;
			element_list <- elem::element_list
	end


class dwindow ?(title = "DrawCaml Window") ?(pos = (10,10)) ?(size = (100,100)) () =
	object
		val main_container = new container
		method getTitle () =
			title
		method getSize () =
			size
		method getPos () =
			pos
		method draw () =
			Printf.printf "This window is called %s, its size is %d,%d and its position is %d,%d \n" (title) (fst size) (snd size) (fst pos) (snd pos)
	end