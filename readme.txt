Gamebuino Library for Arduboy

## Description
Arduboy can play Gamebuino. But probably all game don't work.
It support Gamebuino.h, Gamebuino.cpp. unsupport is SD card Library and etc.


## Games
I tested few game. thank you awesome game! :)

- 101 STARSHIPS by zoglu(sketch size is too big. I del sound. sry.)
- CRABATOR by rodot
- UFO-RACE by rodot
- INVADERS by yoda
- PAQMAN by yoda
- WHG by sorunome

http://gamebuino.com/wiki/index.php?title=Games


## Controls
Gamebuino exist C Button. C Button of Arduboy is A + B.


## Technical information
Game source code change define.

before:

    #include <Gamebuino.h>

after:

    #include "Gamebuino/Gamebuino.h"

IDE push compile button. probably it can running.


## Development
compiler : windows Arduino IDE 1.6.9
