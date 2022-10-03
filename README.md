# DrawCaml


![DrawCaml logo](https://cdn.discordapp.com/attachments/1017077725418246264/1017365387131760732/telechargement.png)

# Project organisation

```
├── bin
│   ├── dune
│   └── main.ml           // where we call the DrawCaml library from OCaml
├── DrawCaml.opam
├── dune-project   
├── lib
│   ├── discover
│   │   ├── discover.ml
│   │   └── dune
│   ├── DrawCaml.ml       // the root of the library
│   ├── dune
│   ├── testc.cpp         // the root of the C++ interface
│   ├── window.cpp
│   └── window.h
├── makefile
├── README.md
└── test
    ├── dune
    └── ProjetIntegre.ml  // these OCaml files test the library
```

# Dependencies

- x11 library
- dune to build the project (opam)
- dune-configurator (opam)

# Running the project

Once dune is installed, you can build the library with `dune build`. To compile the *main.ml* file to access the lib, do `dune build bin/main.exe` and do `dune exec ./bin/main.exe` to execute it.
