# Project Warrior

The actual main project of DWARVES group. It's a fighting game, mainly inspired by the Super Smash Bros games by Nintendo.

You can find the Game Design Document <a href="https://github.com/DWARVES/Warrior-GDD">there</a>.

## Licence

See the LICENCE file.

## Building

To build this game you need cmake and the following libraries :
<ul>
<li> SDL2 and its extensions SDL2\_net and SDL2\_image
<li> Box2D
<li> Boost
<li> Lua
</ul>

You should type the following commands on linux (Window and Mac versions are not available yet) to build the program :
<ul>
<li> git clone https://github.com/DWARVES/Project-Warrior.git warrior
<li> mkdir warrior/build
<li> cd warrior/build
<li> cmake -DTESTS=0 ..
<li> make
</ul>

Replace -DTESTS=0 par -DTESTS=1 if you want to compile the tests.

