DESCRIPTION
==================================================
The ```nblists``` library - a library to identify potential neigbor particles using Linked-Cell Lists with a O(N) time cost.
The library may find a use in all sorts of particle-based simulations with short-ranged interactions, such as:
[Molecular dynamics](https://en.wikipedia.org/wiki/Molecular_dynamics) for [Granular Materials](https://en.wikipedia.org/wiki/Granular_material), or [Smoothed-particle hydrodynamics](https://en.wikipedia.org/wiki/Smoothed-particle_hydrodynamics).

`nblists` library is used in [rndmesh](https://github.com/pgniewko/rndmesh) project.

GETTING THE CODE
==================================================
* To get the code:
```
git clone git@github.com:pgniewko/nblists.git
```

* To obtain the most recent version of the code:
```
git pull origin master
```

COMPILING AND INSTALLATION - UNIX
==================================================

* To build a library by typing:
```
make
```

* To install in /usr/local directory (requires a root access):
```
make install
```

USAGE - EXE file
==================================================

* To compile main.cpp file run in a command line (once you've installed the library):
```
make exe
```

The command will result with a binary file run_nbody. Execute this file to run an exemplary simulation.

* To run the exe file type in a command line (refer the code snippet below for the meaning of the arguments):
```
./run_nblists  
```

* An example of how to use the ```nblists``` library: 
```C++
...

```

LICENSE
=====
The library is open-source. If you want to cite the library in any published work please contact me at
gniewko.pablo@gmail.com for an information about credits or check my [website](http://meetpg.pl).

COPYRIGHT NOTICE
================
Copyright (C) 2018, Pawel Gniewek  
Email  : gniewko.pablo@gmail.com    
All rights reserved.  
License: BSD 3  


REFERENCES
===============
1. "Computer Simulations of Liquids", M.P. Allen, D.J. Tildesley, 1987, Oxford University Press, Chapter 5.3
2. "Understanding Molecular Simulation: From Algorithms to Applications", D.Frenkel, B.Smit.2002, Elsevier, Appendix:F
