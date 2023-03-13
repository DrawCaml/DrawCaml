# C++ documentation of DrawCaml's internal code:

## Structure:

The internal code for the library is inside the ```lib``` folder of the repository. 

```
├── lib                     // code of drawcaml library
|   |
│   ├── action.cpp  		// interaction with C++ queue
│   ├── action.h
|   |
│   ├── container.cpp   	// code for Drawcaml.dcontainer
│   ├── container.h
|   |
│   ├── drawcaml.cpp 		// methods to interface our C++ code with our OCaml code
│   ├── drawCaml.ml   		// OCaml part of the lib + wrappers to drawcaml.cpp methods
|   |
│   ├── element.cpp 		// code for abstract object Drawcaml.delement
│   ├── element.h
|   |
│   ├── utils.h  			// Some utilitary functions: LOG, WARNING, ERROR...
|   |
│   ├── window.cpp 			// code for the window
│   └── window.h
```

## Objects

C++ code keeps the exact same objects as created from OCaml side.

The definition and interactions of these objects can be seen [here](hierarchy.html).
