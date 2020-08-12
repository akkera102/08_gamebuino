Gamebuino Library for Arduboy


## Description
Arduboy can play Gamebuino. But probably all game don't work.
It support only Gamebuino library.
unsupport is SD card Library and etc.


## Games
I tested few game. thank you awesome game! :)

- 101 STARSHIPS by zoglu(sketch size is too big. no logo image)
- AGEDDON by wuuff
- CRABATOR by rodot
- UFO-RACE by rodot
- INVADERS by yoda
- PAQMAN by yoda
- WHG by sorunome
- JEZZBALL by rackhamlenoir
- BIGBLACKBOX by studiocraftapps
- DIGGER by sutchig(sketch size is too big. no sound)
- SOLITAIRE by aoneill


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

3. If you don't hear sound, change NUM_CHANNELS in lib_settings.c.


## Development
compiler : windows Arduino IDE 1.8.13


## History
v1.04 2020/08/12    fork DIGGER arduboy version.thank you eried!
v1.03 2020/08/12    fix eeprom problem
                    add some games
                    PAQMAN,WHG fps 30 -> 20
v1.03 2017/10/21    bit fix. clean up souce code
v1.02 2017/02/07    fix 101 STARSHIPS(add music)
v1.01 2016/10/28    fix compile problem
                    add Armageddon Game
v1.00 2016/--/--    first version


## Licence

Library: LGPL3
https://github.com/Rodot/Gamebuino

Game: Please check with each game.
http://legacy.gamebuino.com/wiki/index.php?title=Games


