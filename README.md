# m-calc_ver.2
This is just my personal implmentation of a calculator in C. Feel free to use it in your own projects.
**THIS PROJECT DOES NOT PROPERLY. SEE M-CALC VER.3 INSTEAD**

This is a minimal calculator library implmented completely in C. It is designed to have as few dependencies as possible, making it more portable.

## How To Use
Requirements: `make` and `gcc` installed on your system

Steps:
1. Clone this repository onto your system using `git clone https://github.com/BlueHatCyberSpy/m-calc_ver.2.git`
2. Run `cd m-calc_ver.2` then run `make`.
3. Run the executable by typing `./mcalc2 "{expression}"` and replace {expression} with the actual expression that you want to evaluate. __Note that the quotation marks are important, and it can lead to errors if you don't wrap the expression with them.__

Alternatively, if you want to use the source code in your own project, you can just copy the "mcalc2.c", "mcalc2.h", and "arrfuncs.h" files into the project directory, the include the mcalc2.o (generated using the command `gcc -c mcalc2.c`) object file into the final compilation. Include the "mcalc2.h" file anywhere where you want to use the functions.
