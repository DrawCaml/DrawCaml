# DrawCaml


![DrawCaml logo](https://cdn.discordapp.com/attachments/1017077725418246264/1017365387131760732/telechargement.png)

# What is DrawCaml ?

DrawCaml is a graphical module for OCaml, built for Unix architectures. It lays on X11 library, and is developped using C++.

Our motivation was to make easier the creation of graphical applications with OCaml, by using Object Oriented part of OCaml, and make its use closer to modern graphical modules that exists in other languages. 

# Documentation

Documentation can be found on [Github Pages](https://drawcaml.github.io/DrawCaml)

# Project organisation

```
├── bin                     // example files to test the library
│   ├── dune
│   ├── main.ml             // a simple code that contains all the features
│   └── snake.ml            // a simple snake game
├── docs                    // project documentation
├── doxygen_config
├── DrawCaml.opam
├── dune-project
├── lib                     // code of drawcaml library
│   ├── discover
│   │   ├── discover.ml
│   │   └── dune
│   ├── action.cpp
│   ├── action.h
│   ├── container.cpp
│   ├── container.h
│   ├── CPP_Doc.md
│   ├── drawcaml.cpp
│   ├── drawCaml.ml
│   ├── dune
│   ├── element.cpp
│   ├── element.h
│   ├── test.cpp
│   ├── utils.h
│   ├── window.cpp
│   └── window.h
├── makefile                // makefile to build documentation 
├── ocamldoc.out
├── ocamldoc.sty
└── README.md
```

# Dependencies

- x11 library
- [opam](https://opam.ocaml.org/doc/Install.html)
- dune and dune-configurator (`opam install dune dune-configurator`)

# Running the project

Once all dependencies are installed, you can build the library with `dune build`. 
To compile the *main.ml* file to access the lib, do `dune build bin/main.exe`.
To compile and run it, do `dune exec ./bin/main.exe`.
The same commands stands for the *snake.ml* file.
