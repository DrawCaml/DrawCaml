doc:
	doxygen doxygen_config
	ocamldoc -html -d docs lib/drawCaml.ml
	cp docs/custom_index.html docs/index.html
