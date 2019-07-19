# CubiGrav
In CubiGrav your goal is to get the furthest without touching any obstacle.
To do this you can change your position from the roof to the ceiling using th Up and Down keys.

Compiling for Linux
-------------------

Dependencies: `libSDL2`, `libSDL2_ttf`, `gcc` and `make`
When you have all the dependencies installed run `make` and it will output a binary in the root folder,
if you want a installer run `make dist`, it will output a directory and its compressed counterpart in a `build` directory.

Compiling for Windows
---------------------
The make file asumes you are cross-compiling from linux for Windows but you can change the `MINGW` variable in the Makefile to use the mingw command on Windows, then run `make win` and it will output a `windows` directory
