doc:
	doxygen doxygen_config
	ocamldoc -html -d docs/ocaml lib/drawCaml.ml
