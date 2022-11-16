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
		method virtual draw : int -> unit
	end

class container () =
	object
		inherit element ()
		val mutable element_list = ([] : element list)
		method draw i =
			Printf.printf "We are in a new container which contains : \n\n";
			List.iter ( fun x -> print_string (String.concat "" (List.init (i+1) (fun x -> "    "))); x#draw (i+1)) element_list;
			print_string (String.concat "" (List.init i (fun x -> "    "))) ; Printf.printf "End of the container \n\n";
		method addElem : 'a. (#element as 'a) -> int*int -> unit = fun elem pos_elem ->
			let elem_coerce = (elem :> element) in
			elem_coerce#setPos pos_elem;
			element_list <- elem_coerce::element_list
	end

class dwindow ?(title = "DrawCaml Window") ?(pos = (10,10)) ?(size = (100,100)) () =
	object
		val main_container = new container ()
		method getTitle () =
			title
		method getSize () =
			size
		method getPos () =
			pos
		method getContainer () =
			main_container
		method draw () =
			Printf.printf "\nThis window is called \"%s\", its size is %d,%d and its position is %d,%d \n\n" (title) (fst size) (snd size) (fst pos) (snd pos);
			Printf.printf "This window contains the following objects : \n\n";
			main_container#draw 0

	end



class dbutton () = 
	object
		inherit element ()
		val mutable label = ""
		method setLabel t =
			label <- t
		method draw i =
			print_string("This is a button labeled \""^label^"\"\n\n")
	end