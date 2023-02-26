(lang dune 3.4)

(name DrawCaml)

(generate_opam_files true)

(source
 (github DrawCaml/DrawCaml))

(authors "K.Barbe E.Galvez D.Lesens A.Obrecht")

(license LICENSE)

(documentation https://github.com/DrawCaml/DrawCaml/wiki)

(package
 (name DrawCaml)
 (synopsis "Graphical Library for OCaml projects")
 (description "DrawCaml is a graphical library based on C++ XLib.")
 (depends ocaml dune)
)