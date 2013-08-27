# Project Warrior

Project Warrior is a fighting game occuring in a brand new original universe, with gameplay inspired by Super Smash Bros. games from Nintendo.
This is the actual main project of DWARVES organization, as its first major one.

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

Replace -DTESTS=0 by -DTESTS=1 if you want to compile the tests.

