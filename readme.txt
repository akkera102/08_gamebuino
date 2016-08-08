Gamebuino Library for Arduboy


!! Arduino IDE 1.6.10 can not compile. pls use 1.6.9 !! 

## Description
Arduboy can play Gamebuino. But probably all game don't work.
It support only Gamebuino library.
unsupport is SD card Library and etc.


## Games
I tested few game. thank you awesome game! :)

- 101 STARSHIPS by zoglu(sketch size is too big. I del logo and sound. sry.)
- CRABATOR by rodot
- UFO-RACE by rodot
- INVADERS by yoda
- PAQMAN by yoda
- WHG by sorunome

http://gamebuino.com/wiki/index.php?title=Games


## Controls
Gamebuino exist C Button. C Button of Arduboy is Left + Right.


## Technical information
Game source code change define.

before:

    #include <Gamebuino.h>

after:

    #include "Gamebuino/Gamebuino.h"

probably it can run.


## Development
compiler : windows Arduino IDE 1.6.9


## Licence

Library: LGPL3
https://github.com/Rodot/Gamebuino

Game: Please check with each game.
http://gamebuino.com/wiki/index.php?title=Games


