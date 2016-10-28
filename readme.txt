Gamebuino Library for Arduboy


## Description
Arduboy can play Gamebuino. But probably all game don't work.
It support only Gamebuino library.
unsupport is SD card Library and etc.


## Games
I tested few game. thank you awesome game! :)

- 101 STARSHIPS by zoglu(sketch size is too big. I del logo and music. sry.)
- AGEDDON by wuuff
- CRABATOR by rodot
- UFO-RACE by rodot
- INVADERS by yoda
- PAQMAN by yoda
- WHG by sorunome

http://gamebuino.com/wiki/index.php?title=Games


## Controls
Gamebuino exist C Button. C Button of Arduboy is Left + Right.


## Technical information
1. Copy lib_*.* files.
2. Game source code change define.

before:

    #include <Gamebuino.h>

after:

    #include "lib_Gamebuino.h"

probably it can run.


## Development
compiler : windows Arduino IDE 1.6.12


## History
v1.01 2016/10/28    fix compile problem
                    add Armageddon Game
v1.00 2016/--/--    first version


## Licence

Library: LGPL3
https://github.com/Rodot/Gamebuino

Game: Please check with each game.
http://gamebuino.com/wiki/index.php?title=Games


