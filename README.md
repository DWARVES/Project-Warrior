# Project Warrior

Project Warrior is a fighting game occuring in a brand new original universe, with gameplay inspired by Super Smash Bros. games from Nintendo.
This is the actual main project of DWARVES organization, as its first major one.

You can find the Game Design Document [there](https://github.com/DWARVES/Warrior-GDD).

## Licence

All files that have a description of the licence have the one described. If a file doesn't have any indication of this kind, consider it's under GNU GPLv3 (see LICENCE file).

## Building

To build this game you need cmake and the following libraries :
 - `SDL2` and its extension `SDL2_image` and `SDL2_mixer`.
 - `Boost` for `boost.regex`.
 - `dirent.h` (POSIX header, but can also be found under windows) with `sys/stat.h` or `boost.filesystem`.
 - `FFMPEG`.
 - `OpenGL 2`.

You should type the following commands on Linux (Window and Mac versions are not available yet) to build the program :
```
git clone https://github.com/DWARVES/Project-Warrior.git warrior
mkdir warrior/build
cd warrior/build
cmake -DTESTS=0 ..
make
```

*Please note that the project is still under development, so the source code may not compile.*

Replace `-DTESTS=0` by `-DTESTS=1` if you want to compile the tests.

A documentation of the code source (only meant for developpers) can also be generated with the following command (you will need Doxygen) : `make doc` You'll find the generated documentation in warrior/build/doc in html and pdf versions.

