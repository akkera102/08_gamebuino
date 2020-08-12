/* 
 * (C) Create by STUDIOCRAFTapps
 * 
 * You are free to modify this code as long as you keep this message.
 * Do not sell this game. Feel free to copy 'parts' of the game to 
 * help you create your.
 * 
 * Vous etes libre de modifier ce code aussi longtemps que vous garder
 * ce message. Vous ne devez pas vendre ce jeu. N'hésitez pas à copier
 * des parties de ce jeu pour créer le votre.
 * 
 */

#include "lib_Backlight.h"
#include "lib_Battery.h"
#include "lib_Buttons.h"
#include "lib_Display.h"
#include "lib_Gamebuino.h"
#include "lib_Sound.h"

#define EEPROM_SAVE_START   16

#include <SPI.h>
#include <EEPROM.h>

#include <math.h>
//#include <string>

Gamebuino gb;

int CamX;
int CamY;
int Frame;

byte Mode = 0;

byte NbrOfLevel = 13;
byte LevelsUnlock = 0;
byte MapCursor = 0;

byte KeysGot = 0;

byte EnnemieCount;

byte CurrentLoadedMap = 0;

byte Scroll = 0;
bool Found;

boolean last = false;
boolean AC = false;
byte ACT;
boolean PistonPressed = false;

const byte TeleporterSerie1X[] PROGMEM {
  12,
  0,
  11,
  10
};
const byte TeleporterSerie1Y[] PROGMEM {
  7,
  3,
  15,
  13
};
const byte TeleporterSerie2X[] PROGMEM {
  12,
  0,
  11,
  11
};
const byte TeleporterSerie2Y[] PROGMEM {
  1,
  8,
  11,
  2
};

byte TeleporterCount = 4;

byte* KeyX;
byte* KeyY;
bool* KeyGot;
byte KeyCount;

byte* LockerX;
byte* LockerY;
bool* LockerGot;
byte LockerCount;

/////////////////////////////////////
////Map stuf and sprite drawing V////
/////////////////////////////////////

const byte Map0Preview[] PROGMEM = {
  2,0,0,
  3,0,9,
  2,1,1
};

const byte Map1Preview[] PROGMEM = {
  2,0,0,
  3,0,2,
  0,0,3
};

const byte Map2Preview[] PROGMEM = {
  0,0,2,
  19,0,20,
  10,12,11
};

const byte Map3Preview[] PROGMEM = {
  0,0,0,
  23,0,9,
  2,26,2
};

const byte Map4Preview[] PROGMEM = {
  21,0,21,
  3,0,3,
  3,26,3
};

const byte Map5Preview[] PROGMEM = {
  2,1,2,
  15,0,28,
  27,1,4
};

const byte Map6Preview[] PROGMEM = {
  0,0,21,
  0,0,2,
  26,0,3
};

const byte Map7Preview[] PROGMEM = {
  0,0,0,
  0,9,0,
  21,21,21
};

const byte Map8Preview[] PROGMEM = {
  0,0,0,
  0,9,0,
  10,12,11
};

const byte Map9Preview[] PROGMEM = {
  0,30,0,
  0,29,0,
  0,14,0
};

const byte Map10Preview[] PROGMEM = {
  0,35,0,
  0,35,0,
  39,37,40
};

const byte Map11Preview[] PROGMEM = {
  35,30,0,
  33,34,0,
  23,23,23
};

const byte Map12Preview[] PROGMEM = {
  0,48,49,
  31,50,51,
  33,32,0
};

const byte* MapsPreviews[13] = {
  Map0Preview,
  Map1Preview,
  Map2Preview,
  Map3Preview,
  Map4Preview,
  Map5Preview,
  Map6Preview,
  Map7Preview,
  Map8Preview,
  Map9Preview,
  Map10Preview,
  Map11Preview,
  Map12Preview
};

const byte SpawnCoordX[] PROGMEM = {
  10,
  2,
  7,
  9,
  2,
  7,
  2,
  4,
  1,
  2,
  3,
  1,
  1
};

const byte SpawnCoordY[] PROGMEM = {
  5,
  9,
  7,
  17,
  25,
  3,
  10,
  2,
  9,
  12,
  15,
  6,
  3
};

const byte MapSizeX[] PROGMEM = {
  13,
  19,
  16,
  21,
  12,
  16,
  21,
  9,
  19,
  20,
  19,
  19,
  22
};

const byte MapSizeY[] PROGMEM = {
  9,
  11,
  12,
  19,
  27,
  17,
  12,
  22,
  13,
  15,
  19,
  10,
  23
};



const byte Logo[] PROGMEM = {64,30,0xAD,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xB5,0x0,0x0,0x0,0x77,0x70,0x0,0x0,0xAD,0x0,0x0,0x0,0x52,0x40,0x0,0x0,0xB5,0x0,0x0,0x0,0x62,0x50,0x0,0x0,0xAD,0x0,0x0,0x0,0x52,0x50,0x0,0x0,0xB5,0x0,0x0,0x0,0x77,0x70,0x0,0x0,0xAD,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xB5,0x0,0x0,0x0,0x74,0x27,0x50,0x0,0xAD,0x0,0x0,0x0,0x54,0x54,0x50,0x0,0xB5,0x0,0x0,0x0,0x64,0x74,0x60,0x0,0xAD,0x0,0x0,0x0,0x54,0x54,0x50,0x0,0xB5,0x0,0x0,0x0,0x77,0x57,0x50,0x0,0xAD,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xB5,0x0,0x0,0x0,0x77,0x50,0x0,0x0,0xAD,0x0,0xF,0xF0,0x55,0x50,0x0,0x0,0xB5,0x0,0xF,0xF0,0x65,0x20,0x20,0x0,0xAD,0x0,0xD,0xD0,0x55,0x50,0x0,0x0,0xB5,0x0,0xD,0xD0,0x77,0x50,0x40,0x0,0xAD,0x0,0xF,0xF0,0x0,0x0,0x20,0x0,0xB5,0x0,0xF,0xF0,0xFF,0x0,0xF0,0x0,0xAD,0x0,0xF,0xF0,0x81,0x0,0x60,0x0,0xB5,0x0,0xF,0xF0,0xBD,0x0,0x60,0x0,0xFF,0xFF,0xFF,0xFF,0xA5,0x0,0xF0,0x0,0xC3,0xA6,0x66,0x65,0xA5,0x1,0xB8,0x0,0xA5,0xB9,0x99,0x9D,0xA5,0x3,0x9C,0x0,0x99,0xA6,0x66,0x65,0xA5,0x3,0xC,0x0,0x99,0xB9,0x99,0x9D,0xA5,0x1,0xF8,0x0,0xA5,0xA6,0x66,0x65,0xA5,0x0,0x0,0x0,0xC3,0xB9,0x99,0x9D,0xA5,0x0,0x0,0x0,0xFF,0xA6,0x66,0x65,0xE7,0x0,0x0,0x0,};

const PROGMEM byte Back0[]
{
  8,8,
  B01000000,
  B00000100,
  B00000000,
  B00000000,
  B01000000,
  B00000000,
  B00010000,
  B00000001,
};

const PROGMEM byte Back1[]
{
  8,8,
  B00001000,
  B01000000,
  B00000000,
  B00010000,
  B00000001,
  B00000000,
  B00100000,
  B10000000,
};

const PROGMEM byte ExclPoint[]
{
  8,8,
  B10000000,
  B10000000,
  B10000000,
  B10000000,
  B10000000,
  B10000000,
  B00000000,
  B10000000,
};

const PROGMEM byte WhitePotion[]
{
  8,8,
  B00111100,
  B00011000,
  B00011000,
  B01101110,
  B10000101,
  B10100001,
  B10001001,
  B01111110,
};

const PROGMEM byte PlayerSprite[]
{
  8,8,
  B11111111,
  B11111111,
  B11101101,
  B11101101,
  B11111111,
  B11111111,
  B11111111,
  B11111111,
};

const PROGMEM byte EArrow1[]
{
  8,8,
  B00000000,
  B01000000,
  B01100000,
  B01110000,
  B01110000,
  B01100000,
  B01000000,
  B00000000,
};

const PROGMEM byte EArrow2[]
{
  8,8,
  B00000000,
  B00000010,
  B00000110,
  B00001110,
  B00001110,
  B00000110,
  B00000010,
  B00000000,
};

const PROGMEM byte ELocked[]
{
  8,8,
  B00000000,
  B00110000,
  B01001000,
  B01111000,
  B01111000,
  B01111000,
  B00000000,
  B00000000,
};

const PROGMEM byte EUnLocked[]
{
  8,8,
  B00000000,
  B00000110,
  B00001001,
  B01111000,
  B01111000,
  B01111000,
  B00000000,
  B00000000,
};

const PROGMEM byte Key[] = //ID: 19
{
  8,8,
  B00000000,
  B00000110,
  B00001111,
  B11111101,
  B10101111,
  B00000110,
  B00000000,
  B00000000,
};

const PROGMEM byte LockedBlock[] = //ID: 20
{
  8,8,
  B01111110,
  B11011111,
  B10100011,
  B10100011,
  B11110111,
  B10110111,
  B11110111,
  B01111110,
};

const PROGMEM byte Empty[] = //ID: 0
{
  8,8,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
};

const PROGMEM byte Missing[]
{
  8,8,
  B10101010,
  B00000001,
  B10000000,
  B00000001,
  B10000000,
  B00000001,
  B10000000,
  B01010101,
};

const PROGMEM byte GroundLeft[] = //ID: 10
{
  8,8,
  B11111111,
  B10111001,
  B10100110,
  B10111001,
  B10100110,
  B10111001,
  B10100110,
  B10111001,
};

const PROGMEM byte GroundRight[] = //ID: 11
{
  8,8,
  B11111111,
  B10011101,
  B01100101,
  B10011101,
  B01100101,
  B10011101,
  B01100101,
  B10011101,
};

const PROGMEM byte GroundMiddle[] = //ID: 12
{
  8,8,
  B11111111,
  B10011001,
  B01100110,
  B10011001,
  B01100110,
  B10011001,
  B01100110,
  B10011001,
};

const PROGMEM byte Window[] = //ID: 13
{
  8,8,
  B11111111,
  B10000001,
  B10000001,
  B10000001,
  B10000001,
  B10000001,
  B10000001,
  B11111111,
};

const PROGMEM byte Brick[] = //ID: 1
{
  8,8,
  B11111111,
  B00100001,
  B11111111,
  B10001000,
  B11111111,
  B00100001,
  B11111111,
  B10001000,
};

const PROGMEM byte SharpBrick[]= //ID: 2
{
  8,8,
  B11111111,
  B11000011,
  B10100101,
  B10011001,
  B10011001,
  B10100101,
  B11000011,
  B11111111,
};
const PROGMEM byte TowerBrick[]= //ID: 3
{
  8,8,
  B10101101,
  B10110101,
  B10101101,
  B10110101,
  B10101101,
  B10110101,
  B10101101,
  B10110101,
};

const PROGMEM byte Piston[]= //ID: 4
{
  8,8,
  B11111111,
  B10000001,
  B11011011,
  B11011011,
  B00111100,
  B11100111,
  B10000001,
  B11111111,
};

const PROGMEM byte Holder[]= //ID: 5
{
  8,8,
  B11111111,
  B10100101,
  B11111111,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
};

const PROGMEM byte  Teleporter1[]= //ID: 6
{
  8,8,
  B11111111,
  B10101011,
  B11010110,
  B10101100,
  B10101100,
  B11010110,
  B10101011,
  B11111111,
};

const PROGMEM byte Teleporter0[]= //ID: 7
{
  8,8,
  B11111111,
  B11010101,
  B01101011,
  B00110101,
  B00110101,
  B01101011,
  B11010101,
  B11111111,
};

const PROGMEM byte Wall[]= //ID: 8
{
  8,8,
  B11110000,
  B10010000,
  B10010000,
  B10010000,
  B10010000,
  B10010000,
  B10010000,
  B11110000,
};

const PROGMEM byte Ennemie[]= //ID: 9
{
  8,8,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
};

const PROGMEM byte End[]= //ID: 22
{
  8,8,
  B00000000,
  B00110011,
  B11001100,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
};

const PROGMEM byte EnnemieSprite[]=
{
  8,8,
  B11111111,
  B10000001,
  B10010011,
  B10010011,
  B10000001,
  B10000001,
  B10000001,
  B11111111,
};
const PROGMEM byte EnnemieSprite1[]=
{
  8,8,
  B11111111,
  B10000001,
  B11001001,
  B11001001,
  B10000001,
  B10000001,
  B10000001,
  B11111111,
};

const PROGMEM byte Table[] = //ID: 14
{
  8,8,
  B11111111,
  B10000001,
  B10111101,
  B10100101,
  B10100101,
  B10100101,
  B10100101,
  B11100111,
};

const PROGMEM byte PistonExtension[] = //ID: 15
{
  8,8,
  B11100111,
  B10011001,
  B11100111,
  B10011001,
  B11100111,
  B10011001,
  B11100111,
  B10011001,
};

const PROGMEM byte Arrow1[]= //ID: 16
{
  8,8,
  B11111110,
  B10010100,
  B10111000,
  B11111100,
  B10111110,
  B11011111,
  B10001111,
  B00000110,
};

const PROGMEM byte Arrow2[]= //ID: 17
{
  8,8,
  B01111111,
  B00101111,
  B00010111,
  B00101111,
  B01011111,
  B10111011,
  B11110001,
  B01100000,
};

const PROGMEM byte PlantPot[] = //ID: 18
{
  8,8,
  B00000000,
  B00101000,
  B00010100,
  B00001000,
  B00010000,
  B00111100,
  B00111100,
  B00011000,
};

const PROGMEM byte Spike[] = //ID: 21
{
  8,8,
  B00000000,
  B00100010,
  B00100010,
  B01010101,
  B01010101,
  B10001000,
  B10101010,
  B10101010,
};

const PROGMEM byte ConveyerBeltR[] = //ID: 23
{
  8,8,
  B01111110,
  B10100011,
  B11000101,
  B01111110,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
};

const PROGMEM byte ConveyerBeltL[] = //ID: 24
{
  8,8,
  B01111110,
  B11000101,
  B10100011,
  B01111110,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
};

const PROGMEM byte TowerBrickHat[]= //ID: 25
{
  8,8,
  B00000000,
  B00000000,
  B00000000,
  B11111111,
  B10000001,
  B11111111,
  B10101101,
  B10110101,
};

const PROGMEM byte Trampoline[] = //ID: 26
{
  8,8,
  B11111111,
  B00100100,
  B01000010,
  B10000001,
  B01000010,
  B00100100,
  B11111111,
  B11111111,
};

const PROGMEM byte AntiPiston[]= //ID: 27
{
  8,8,
  B11111111,
  B10000001,
  B11000011,
  B00100100,
  B00111100,
  B01000010,
  B10000001,
  B11000011,
};

const PROGMEM byte Potion[]= //ID: 29
{
  8,8,
  B00111100,
  B00011000,
  B00011000,
  B00111100,
  B01101110,
  B11100111,
  B11000011,
  B01111110,
};

const PROGMEM byte Bubble[]= //ID: 30
{
  8,8,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00001000,
  B00000000,
  B00010000,
  B00001000,
};

const PROGMEM byte Pipe0[]= //ID: 31
{
  8,8,
  B00000000,
  B00000000,
  B00001111,
  B00010000,
  B00100000,
  B00100011,
  B00100100,
  B00100100,
};

const PROGMEM byte Pipe1[]= //ID: 32
{
  8,8,
  B00000000,
  B00000000,
  B11110000,
  B00001000,
  B00000100,
  B11000100,
  B00100100,
  B00100100,
};

const PROGMEM byte Pipe2[]= //ID: 33
{
  8,8,
  B00100100,
  B00100100,
  B00100011,
  B00100000,
  B00010000,
  B00001111,
  B00000000,
  B00000000,
};

const PROGMEM byte Pipe3[]= //ID: 34
{
  8,8,
  B00100100,
  B00100100,
  B11000100,
  B00000100,
  B00001000,
  B11110000,
  B00000000,
  B00000000,
};

const PROGMEM byte Pipe4[]= //ID: 35
{
  8,8,
  B00100100,
  B00100100,
  B00100100,
  B00100100,
  B00100100,
  B00100100,
  B00100100,
  B00100100,
};

const PROGMEM byte Pipe5[]= //ID: 36
{
  8,8,
  B00000000,
  B00000000,
  B11111111,
  B00000000,
  B00000000,
  B11111111,
  B00000000,
  B00000000,
};

const PROGMEM byte Stock0[]= //ID: 37
{
  8,8,
  B01000010,
  B10000001,
  B00000000,
  B10011001,
  B01100110,
  B00000000,
  B11111111,
  B00000000,
};

const PROGMEM byte Stock1[]= //ID: 38
{
  8,8,
  B00000000,
  B11111111,
  B00000000,
  B10011001,
  B01100110,
  B00000000,
  B11111111,
  B00000000,
};

const PROGMEM byte Stock2[]= //ID: 39
{
  8,8,
  B00000000,
  B01111111,
  B10000000,
  B10011001,
  B11100110,
  B10000000,
  B01111111,
  B00000000,
};

const PROGMEM byte Stock3[]= //ID: 40
{
  8,8,
  B00000000,
  B11111110,
  B00000001,
  B10011001,
  B01100111,
  B00000001,
  B11111110,
  B00000000,
};

const PROGMEM byte Controller0[]= //ID: 41
{
  8,8,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B11100000,
};

const PROGMEM byte Controller1[]= //ID: 42
{
  8,8,
  B10110000,
  B10101000,
  B10101100,
  B10101010,
  B10101001,
  B11111111,
  B10000001,
  B11111111,
};

const PROGMEM byte Crate[]= //ID: 43
{
  8,8,
  B00000000,
  B00000000,
  B01111110,
  B01000010,
  B01110110,
  B01101110,
  B01000010,
  B01111110,
};

const PROGMEM byte Experiment0[]= //ID: 44
{
  8,8,
  B00001111,
  B00010000,
  B00010011,
  B00011100,
  B00010000,
  B00010000,
  B00010000,
  B00010111,
};

const PROGMEM byte Experiment1[]= //ID: 45
{
  8,8,
  B11110000,
  B00001000,
  B00001000,
  B11001000,
  B00111000,
  B00001000,
  B00001000,
  B11101000,
};

const PROGMEM byte Experiment2[]= //ID: 46
{
  8,8,
  B00010011,
  B00010011,
  B11110111,
  B00000111,
  B00000111,
  B11110110,
  B00010000,
  B00001111,
};

const PROGMEM byte Experiment3[]= //ID: 47
{
  8,8,
  B01101000,
  B01101000,
  B11101000,
  B11101000,
  B11101000,
  B01101000,
  B00001000,
  B11110000,
};

//---

const PROGMEM byte Experiment4[]=
{
  8,8,
  B00001111,
  B00010000,
  B00010011,
  B00011100,
  B00010000,
  B00010111,
  B00010111,
  B00010101
};

const PROGMEM byte Experiment5[]=
{
  8,8,
  B11111100,
  B00000010,
  B00110010,
  B11001110,
  B00000010,
  B11111010,
  B11111010,
  B10111010
};

const PROGMEM byte Experiment6[]=
{
  8,8,
  B00010101,
  B00010111,
  B11110111,
  B00000111,
  B00000111,
  B11110000,
  B00010000,
  B00011111
};

const PROGMEM byte Experiment7[]=
{
  8,8,
  B10111010,
  B11111010,
  B11111010,
  B11111010,
  B11111010,
  B00000010,
  B00000010,
  B11111100
};

/*const PROGMEM byte Name[]= //ID: 40
{
  8,8,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
};*/

const byte Map0[] PROGMEM = {
0,0,0,0,0,0,0,0,0,0,0,0,2,
2,1,2,5,5,0,0,0,0,0,0,0,6,
3,0,0,0,0,0,4,5,2,0,0,0,2,
3,0,25,0,16,0,15,0,0,0,0,0,3,
2,0,3,0,0,0,15,18,0,0,0,0,3,
3,0,3,0,17,0,15,14,0,0,0,0,3,
3,0,3,0,0,0,2,5,5,0,5,5,2,
3,0,3,0,9,0,3,0,9,0,0,0,6,
2,22,2,1,1,1,2,1,1,1,1,1,2,
};

const byte Map1[] PROGMEM = {
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  2,1,2,0,0,0,0,2,1,1,1,1,1,1,1,1,1,1,2,
  3,0,0,0,0,0,0,0,0,8,0,2,0,0,0,0,0,0,3,
  3,0,0,0,21,0,0,0,0,8,0,3,0,0,0,0,0,0,3,
  3,0,0,5,5,5,5,0,0,0,0,2,0,0,5,5,5,0,3,
  2,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,2,
  3,0,0,0,2,0,0,0,3,0,0,0,0,0,0,21,0,0,3,
  3,0,0,0,21,21,21,21,3,0,0,21,0,0,0,2,0,0,3,
  3,0,0,0,2,2,2,2,2,0,5,5,5,0,0,3,0,0,3,
  3,0,0,0,3,0,0,0,3,21,21,21,21,21,21,3,22,22,3,
  2,1,1,1,2,0,0,0,2,1,1,1,1,1,1,2,1,1,2
};

const byte Map2[] PROGMEM = {
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  2,0,0,0,0,8,0,0,0,0,0,2,0,0,0,2,
  3,0,0,0,0,8,0,0,0,0,0,3,0,0,0,3,
  7,0,0,0,0,0,0,0,0,0,0,3,0,0,0,3,
  2,5,0,0,0,0,0,21,19,0,0,3,0,0,0,3,
  3,0,0,0,9,0,21,2,5,0,0,2,0,0,0,3,
  3,1,1,1,1,1,2,5,0,0,0,0,0,0,0,2,
  2,0,0,0,0,0,0,0,0,0,0,0,0,0,19,3,
  7,0,0,19,0,0,0,0,0,21,0,17,0,2,1,2,
  2,1,1,1,1,2,0,5,0,2,5,0,0,3,0,3,
  3,0,20,20,20,0,0,0,0,3,0,0,0,3,0,3,
  2,22,10,12,11,2,10,12,11,2,21,21,21,2,0,2,
};

const byte Map3[] PROGMEM = {
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,
  3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,
  3,0,0,0,0,0,0,0,15,0,15,15,0,15,0,15,15,0,0,0,3,
  3,0,0,0,0,0,0,0,0,15,21,21,21,21,21,15,0,4,0,16,3,
  3,0,0,0,0,0,0,0,0,2,1,1,1,1,1,2,0,0,0,0,3,
  3,0,0,0,0,0,0,15,0,0,0,0,0,0,0,0,0,0,0,0,3,
  3,0,0,0,0,0,0,15,0,0,0,0,0,0,0,0,0,0,2,5,3,
  3,0,21,0,0,21,0,25,0,0,9,0,0,0,21,0,0,0,0,0,3,
  3,0,5,5,5,5,0,3,23,23,23,23,23,23,3,5,0,0,0,0,3,
  2,15,0,0,0,0,27,2,5,5,5,5,5,5,2,0,0,0,0,5,2,
  3,5,0,0,0,0,2,21,0,0,0,0,0,21,0,0,0,0,0,0,3,
  3,4,0,0,0,0,1,2,1,1,1,1,1,2,0,17,0,0,9,0,3,
  3,5,5,5,0,5,15,0,0,0,0,0,0,8,0,25,21,21,21,21,3,
  2,0,0,0,0,2,0,25,0,0,0,0,19,8,0,2,1,2,5,5,2,
  3,0,24,24,24,0,0,3,24,24,24,24,24,8,0,0,0,0,0,0,3,
  3,0,0,0,0,0,0,2,5,5,5,5,5,2,0,0,18,0,25,20,3,
  3,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,14,0,3,22,3,
  2,1,1,1,1,1,26,2,10,12,12,12,11,2,26,2,10,12,12,11,2
};

const byte Map4[] PROGMEM = {
  0,0,0,0,0,0,0,0,0,0,0,0,
  2,1,1,1,1,1,2,0,0,0,0,0,
  3,0,0,0,0,0,3,0,0,0,0,0,
  3,0,0,0,0,0,1,1,2,0,0,0,
  3,0,0,0,0,0,0,0,3,0,0,0,
  3,0,0,0,21,0,0,0,2,1,1,2,
  3,22,2,0,3,0,0,0,0,0,0,3,
  3,2,2,0,3,21,0,0,21,0,0,3,
  3,0,0,0,5,5,5,5,2,0,0,3,
  3,0,0,0,0,0,0,0,3,0,0,3,
  3,0,0,0,0,0,0,0,3,0,0,2,
  3,0,21,21,21,21,21,21,3,0,0,6,
  3,21,2,1,1,1,1,1,1,2,21,3,
  3,2,2,0,0,0,0,0,0,1,1,2,
  2,0,0,0,0,0,0,0,0,0,0,3,
  3,0,0,0,21,0,0,21,0,0,0,6,
  3,0,0,0,2,5,5,2,5,5,5,2,
  3,0,0,0,0,0,0,3,0,0,0,0,
  2,0,0,0,0,0,0,2,1,1,1,2,
  3,0,0,0,0,0,0,0,0,0,0,3,
  3,0,0,0,21,0,0,0,0,0,0,3,
  3,0,21,21,3,0,0,21,0,0,0,3,
  2,1,1,1,2,1,1,2,17,0,0,3,
  3,0,0,0,0,0,0,0,0,0,0,3,
  3,0,0,0,0,0,0,0,0,0,16,3,
  3,0,0,0,0,0,0,0,0,0,0,3,
  2,1,1,1,2,21,21,21,26,21,21,2
};

const byte Map5[] PROGMEM = {
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  2,1,1,1,2,3,2,4,2,3,1,1,1,1,1,2,
  3,0,0,0,28,3,0,0,0,15,0,0,0,0,0,3,
  3,0,8,0,28,20,0,0,0,15,0,0,0,0,0,3,
  2,0,4,1,2,3,2,27,2,3,0,0,0,0,0,3,
  3,0,0,0,0,0,0,0,0,3,15,15,27,28,28,3,
  3,16,0,0,0,0,0,0,0,3,0,0,15,0,0,3,
  3,0,0,0,0,0,0,0,0,3,0,0,15,0,0,3,
  3,0,0,21,0,21,0,0,0,3,28,28,4,15,15,4,
  3,21,21,2,15,2,0,26,0,3,0,0,28,0,0,3,
  3,5,5,5,0,5,5,5,5,3,0,0,28,0,0,3,
  3,0,0,0,0,0,0,0,0,3,0,0,27,28,28,3,
  3,0,0,0,0,18,0,0,0,3,0,0,15,0,0,3,
  3,0,25,0,0,14,0,0,0,3,0,0,15,0,0,3,
  3,0,2,5,5,2,5,20,5,2,1,1,1,2,28,3,
  3,0,0,0,19,3,22,22,22,3,19,0,0,0,0,3,
  2,1,1,1,1,2,1,1,1,2,1,1,1,1,1,2
};

const byte Map6[] PROGMEM = {
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  2,1,1,1,2,1,1,1,1,2,0,0,0,0,0,0,0,0,0,0,2,
  3,0,0,0,0,0,0,0,0,3,21,21,0,0,0,0,0,0,0,0,3,
  3,0,0,0,0,0,0,0,0,2,1,2,0,0,0,0,0,0,0,0,3,
  3,0,0,0,21,0,0,0,0,0,0,8,17,0,0,0,0,0,0,0,3,
  3,0,0,0,2,0,0,0,0,0,0,8,0,21,0,0,21,0,0,0,3,
  3,0,0,0,3,0,0,0,0,21,0,0,0,2,0,0,2,0,0,0,3,
  3,0,0,0,3,21,21,21,21,2,0,0,0,3,0,0,0,0,0,0,3,
  3,0,0,0,3,5,5,5,5,3,0,0,0,3,21,21,21,21,21,0,3,
  3,0,0,0,3,0,0,0,0,3,0,0,0,3,5,5,5,5,2,0,3,
  3,0,26,0,3,0,0,0,0,3,0,26,0,3,0,0,0,0,3,22,3,
  2,10,12,11,2,0,0,0,0,2,10,12,11,2,0,0,0,0,2,5,2
};

const byte Map7 [] PROGMEM = {
  0,0,0,0,0,0,0,0,0,
  2,1,4,1,1,1,1,1,2,
  3,0,0,0,0,0,0,0,3,
  3,28,28,28,28,28,28,28,3,
  3,0,0,0,0,0,0,0,3,
  3,0,0,0,0,0,21,21,3,
  3,0,0,0,0,0,5,5,3,
  3,21,0,0,0,0,0,0,3,
  3,5,21,0,0,0,0,16,3,
  3,5,5,21,0,0,0,21,3,
  3,5,5,5,21,0,21,5,3,
  3,5,5,5,5,0,5,5,3,
  3,0,0,0,0,0,2,0,3,
  3,0,0,0,0,0,3,0,3,
  3,0,21,21,21,21,2,0,3,
  3,0,5,5,5,5,5,0,3,
  3,0,0,0,0,0,0,0,3,
  3,0,0,0,0,0,0,0,3,
  3,0,0,0,0,0,0,0,3,
  3,2,0,0,0,0,0,0,3,
  3,3,0,9,0,0,0,0,3,
  3,2,21,21,21,21,2,22,3,
  2,1,1,1,1,1,1,1,2
};

const byte Map8 [] PROGMEM = {
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,
  3,0,0,0,0,0,3,0,0,0,3,0,0,0,0,0,0,0,3,
  3,0,0,0,0,19,3,0,0,0,20,0,0,0,0,0,25,20,3,
  3,0,5,5,5,5,5,0,0,0,25,0,0,0,0,0,3,22,3,
  3,0,0,0,0,0,0,0,0,0,5,5,5,5,5,5,5,5,2,
  3,0,0,0,25,0,0,0,0,0,0,0,0,0,0,0,0,0,3,
  2,1,1,1,2,0,0,0,0,0,0,0,0,0,0,0,0,0,3,
  3,0,0,0,3,0,0,0,25,0,9,0,25,0,0,0,0,0,3,
  3,0,0,0,5,0,0,0,3,5,5,5,5,0,0,0,0,0,3,
  3,5,5,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,3,
  3,0,0,9,0,0,0,0,3,19,0,0,9,0,0,0,0,0,3,
  2,10,12,11,2,10,12,11,2,10,12,11,2,10,12,11,2,1,2
};

const byte Map9 [] PROGMEM = {
  00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,
  02,04,01,02,01,01,02,01,01,01,02,01,01,01,01,02,00,35,00,00,
  03,00,00,03,00,00,03,27,00,00,03,07,00,15,00,03,00,33,36,32,
  03,00,00,03,00,00,02,00,00,00,02,05,00,15,00,03,00,30,00,35,
  03,00,00,03,17,00,28,00,00,00,00,00,05,02,00,02,00,29,00,35,
  03,00,00,27,00,00,28,00,00,00,00,00,00,03,00,03,00,14,00,35,
  03,00,00,03,00,00,02,00,00,21,00,00,00,03,00,02,01,01,02,35,
  03,00,00,02,00,00,03,05,05,02,21,00,00,02,00,00,00,00,03,35,
  03,00,00,28,00,00,03,00,00,03,05,21,00,03,05,05,05,00,03,35,
  03,00,00,28,00,00,03,32,00,03,05,05,00,03,00,00,00,00,02,35,
  03,00,00,02,21,21,02,35,00,02,00,00,00,02,22,22,22,22,03,35,
  03,00,00,03,05,05,03,35,00,03,00,00,00,03,05,05,05,05,05,35,
  03,00,00,03,00,00,03,35,00,03,00,00,00,03,00,00,31,36,36,34,
  03,21,26,03,00,00,03,33,36,03,07,00,00,03,39,38,37,38,38,40,
  02,10,12,02,00,00,02,00,00,02,01,01,01,02,01,01,01,01,01,01,
};

const byte Map10 [] PROGMEM = {
  00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,
  02,01,01,01,01,02,00,00,00,00,00,00,00,00,00,00,00,00,00,
  03,00,00,20,20,00,00,00,00,00,00,00,00,00,00,00,00,00,00,
  02,22,02,01,01,02,00,00,00,00,00,00,00,00,00,30,00,19,00,
  00,02,00,00,31,32,00,00,00,00,00,00,00,00,00,33,03,05,00,
  00,00,00,00,35,35,00,00,21,00,00,00,00,00,00,00,05,00,00,
  36,36,36,36,34,33,36,36,02,00,9,00,00,02,00,00,00,00,00,
  00,00,39,38,38,40,00,00,05,05,05,05,05,05,00,00,00,00,21,
  00,31,36,32,00,00,00,00,00,00,00,00,00,00,00,00,00,00,02,
  00,35,39,37,38,38,40,00,00,00,00,00,00,00,00,00,00,00,03,
  36,34,00,00,00,00,00,00,00,00,00,21,41,00,00,00,21,00,03,
  00,00,00,00,00,00,00,00,00,00,00,02,42,00,9,00,02,00,03,
  00,00,00,00,00,00,00,00,00,00,00,03,05,05,05,05,03,19,03,
  00,00,00,00,00,00,00,00,00,00,00,03,00,43,00,00,03,05,02,
  36,36,32,00,00,00,00,26,00,00,00,03,00,14,00,00,03,00,00,
  00,39,37,40,00,00,02,01,02,00,00,02,05,05,05,05,02,00,00,
  00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,
  00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,
  21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,
};

const byte Map11 [] PROGMEM = {
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  2,0,0,0,0,35,0,0,0,0,0,0,0,0,0,0,0,0,2,
  3,0,0,0,0,35,0,0,0,0,0,0,0,0,0,0,0,0,3,
  3,22,8,21,0,33,32,0,21,0,0,0,21,0,0,0,0,0,3,
  3,2,1,1,1,2,35,0,5,0,0,2,1,8,0,0,0,0,2,
  3,0,0,0,0,0,35,0,30,0,0,0,0,0,0,0,0,0,3,
  3,0,0,0,0,0,35,0,35,0,0,0,0,0,0,0,0,0,3,
  3,5,0,0,0,0,33,36,34,21,0,0,0,0,0,0,0,0,3,
  3,0,0,0,0,9,0,0,0,3,0,0,21,21,0,25,0,0,3,
  2,23,23,23,23,23,23,23,23,2,23,23,23,23,23,2,21,21,2
};

const byte Map12 [] PROGMEM = {
  0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  2,1,2,1,2,1,2,0,0,0,0,0,0,0,0,0,0,2,0,4,0,2,
  3,0,5,0,0,0,3,0,0,0,0,0,0,0,0,0,0,15,0,0,0,3,
  3,0,0,0,0,0,3,19,0,25,0,9,0,0,8,0,0,15,0,0,0,3,
  2,1/*1*/,2,1,2,0,3,5,5,5,5,5,5,5,8,0,0,2,28,28,28,3,
  0,0,0,0,3,0,3,0,0,0,0,0,0,0,5,0,0,3,0,0,0,3,
  0,0,0,0,3,0,2,0,0,0,0,0,0,0,0,0,0,3,0,0,0,3,
  0,0,0,0,3,0,0,0,0,21,0,0,0,0,0,0,0,3,0,21,0,3,
  0,0,0,0,3,0,21,0,0,3,0,0,21,21,0,25,0,3,0,2,0,3,
  0,0,0,0,3,0,2,23,23,3,23,23,23,23,23,3,5,3,0,0,0,3,
  0,0,0,0,3,21,3,0,0,3,0,43,0,0,0,3,0,3,21,0,21,3,
  2,1/*1*/,1,1,2,5,2,1,1,2,1,1,1,1,1,2,1,2,2,0,2,2,
  3,35,0,0,35,0,0,35,0,0,0,0,0,33,32,3,0,0,0,0,0,3,
  3,35,44,45,35,48,49,35,48,49,0,0,0,31,34,3,0,0,0,0,0,3,
  3,33,46,47,33,50,51,33,50,51,0,0,39,37,40,3,0,21,0,0,0,3,
  3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,2,0,0,0,3,
  3,1,1,1,1,1,1,1,2,0,0,0,0,0,30,3,0,3,0,0,0,3,
  3,0,0,0,0,0,0,0,0,0,0,0,0,0,33,3,0,3,0,0,0,3,
  3,0,0,0,0,0,0,0,0,0,5,5,5,0,0,3,0,3,21,0,21,3,
  3,0,0,30,0,0,0,0,0,0,0,0,0,0,43,3,0,3,5,0,5,3,
  3,0,0,29,0,0,0,41,0,0,0,0,0,0,5,2,0,3,0,0,0,3,
  3,0,0,14,0,9,2,42,0,0,0,0,0,0,0,20,0,3,0,26,0,3,
  2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,1,2,1,1,1,2
};

const byte* GetMap[] {
  Map0,
  Map1,
  Map2,
  Map3,
  Map4,
  Map5,
  Map6,
  Map7,
  Map8,
  Map9,
  Map10,
  Map11,
  Map12
};

//////////////////////////////////////////////
////Var Initi. for physics and grounding V////
//////////////////////////////////////////////

#define LCDWidth 84
#define LCDHeight 48

#define BouncyMath1 0.0322580//6452
#define BouncyMath2 -0.0483870f//9677
#define BouncyMath3 -0.0393700f//7874
#define BouncyMath4 0.0236220f//4724

byte MapHeigth = 9;
byte MapWidth = 13;

boolean GoingRight = true;

boolean GroundedDown = false;
boolean GroundedRight = false;
boolean GroundedLeft = false;

boolean IsPlaying = false;

/////////////////////
////Block Setup V////
/////////////////////

const byte* sprites[52] = {
  Empty,
  Brick,
  SharpBrick,
  TowerBrick,
  Piston,
  Holder,
  Teleporter0,
  Teleporter1,
  Wall,
  Ennemie,
  GroundLeft,
  GroundRight,
  GroundMiddle,
  Window,
  Table,
  PistonExtension,
  Arrow1,
  Arrow2,
  PlantPot,
  Key,
  LockedBlock,
  Spike,
  End,
  ConveyerBeltR,
  ConveyerBeltL,
  TowerBrickHat,
  Trampoline,
  AntiPiston,
  Missing,
  Potion,
  Bubble,
  Pipe0,
  Pipe1,
  Pipe2,
  Pipe3,
  Pipe4,
  Pipe5,
  Stock0,
  Stock1,
  Stock2,
  Stock3,
  Controller0,
  Controller1,
  Crate,
  Experiment0,
  Experiment1,
  Experiment2,
  Experiment3,
  Experiment4,
  Experiment5,
  Experiment6,
  Experiment7
};

const byte* coltype[52] = { //0 = empty 1 = block
  0,
  1,
  1,
  1,
  9,
  2, //2
  3,
  4,
  5, //5
  0,
  1,
  1,
  1,
  0,
  0,
  6,
  0,
  0,
  0,
  7,
  8,
  10,
  11,
  12,
  13,
  14,
  15,
  16,
  17,
  18,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  //0,
  1,
  1,
  1,
  1,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
};

const byte* coltypeSys[52] = { //0 = empty 1 = block
  0,
  1,
  1,
  1,
  1,
  1, //2
  1,
  1,
  1, //5
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1
};

//////////////////////////////
////GetTile & Var initi. V////
//////////////////////////////


byte getTile(byte x, byte y){
    return pgm_read_byte(GetMap[MapCursor] + (x+y*MapWidth)); 
    //return (Maps[CurrentLoadedMap])[x+y*MapWidth];
}

byte getPreviewTile(byte x, byte y){
    return pgm_read_byte(MapsPreviews[MapCursor] + (x+y*3));
    //return MapsPreviews[MapCursor][(x+y*MapWidth)];
} 

///////Sound////////

const int soundfx[8][8] = {
  {1,27,90,2,7,7,3,7}, //jump <
  {1,27,112,1,1,1,6,4}, // walljump <
  {0,27,57,1,1,1,6,4}, // unlockdoor <
  {0,9,57,1,6,8,7,8}, // land
  {0,46,57,1,0,18,7,47}, // getkey <
  {0,38,79,3,6,7,7,20}, // finish <
  {0,30,68,3,0,0,7,5}, // click <
  {0,30,55,1,7,0,7,15} //Death <
};

void sfx(int fxno, int channel) {
  gb.sound.command(0, soundfx[fxno][6], 0, channel); // set volume
  gb.sound.command(1, soundfx[fxno][0], 0, channel); // set waveform
  gb.sound.command(2, soundfx[fxno][5], -soundfx[fxno][4], channel); // set volume slide
  gb.sound.command(3, soundfx[fxno][3], soundfx[fxno][2] - 58, channel); // set pitch slide
  gb.sound.playNote(soundfx[fxno][1], soundfx[fxno][7], channel); // play note
  //WAVEFORM, PITCH, PMD, PMT, VMD, VMT, VOL, LENGTH
}

int CPosX = 0; //Cam Position
int CPosY = 0;

float PPosX = 0; //Player Position
float PPosY = 0;

byte SquisheVertical = 8;      //Player squishness :)
byte SquisheHorizontal = 8;

char VelocityX;    //Velocity (used for physics)
char VelocityY;

///////////////////
////EnnemieAi V////
///////////////////

class EnnemieAI {
  private:
    byte PosX;
    byte PosY;

    byte Progress = 0;
  
    boolean GoRight = true;
    boolean Pre = false;
  
    void ChangeDirection () {
      GoRight = !GoRight;
    }

    bool SimplePixInColl (byte PIMX, byte PIMY) { //PIM = Pos in map (0-255), PIC = Pos in cube (0-8)
      byte GlobalColiderType = coltype[getTile(PIMX,PIMY)];

      if(GlobalColiderType == 0) {
        return false;
      } else if(GlobalColiderType == 16 || GlobalColiderType == 17) {
        return false;
      } else if(GlobalColiderType == 15) {
        return false;
      } else {
        return true;
      }
    }

  public:
    void DefinePos (byte PositionX, byte PositionY) {
      PosX = PositionX;
      PosY = PositionY;
    }

    byte GetPosX () {
      return PosX;
    }

    byte GetPosY () {
      return PosY;
    }

    boolean GetDirection () {
      return GoRight;  
    }

    void update () {
      if(Progress > 2) {
        Progress = 0;
      } else {
        Progress++;
        if(GoRight) {
          if(!SimplePixInColl((floor((PosX + 5) / 8.0)) , (floor((PosY + 3) / 8.0)))) {
            PosX++;
          } else {
            ChangeDirection();
          }
        }
        if(!GoRight) {
          if(!SimplePixInColl((floor((PosX - 5) / 8.0)) , (floor((PosY + 3) / 8.0)))) {
            PosX--;
          } else {
            ChangeDirection();
          }
        }
      }
      Pre = false;
      byte ColY[8];
        for(int yd = 0; yd < 8; yd++) { //yd = Y Down
          ColY[yd] = SimplePixInColl((floor((PosX - 3 + yd) / 8.0)) , (floor((PosY + /*4*/4 + 0) / 8.0)));
        }
      for(int yd = 0; yd < 8; yd++) {
        if(ColY[yd] == 1) {
          Pre = true;
          break;
        }
      }
      if(!Pre) {
        PosY++;
      }
    }
};

EnnemieAI* ennemieArray;

///////////////////////////////
////Setup & PlayerDrawing V////
///////////////////////////////

void setup() {
  // put your setup code here, to run once:
  gb.begin();
  gb.titleScreen(F("v1.0"),Logo);
  gb.battery.show = false;
  gb.setFrameRate(20);
  
  LevelsUnlock = EEPROM.read(EEPROM_SAVE_START + 0);
}

void DrawPlayer () {  //Draw player based on squisness
  if(GoingRight == true) {
    gb.display.fillRect(CPosX - (PPosX + SquisheHorizontal/2.0) + 8, CPosY - (PPosY + SquisheVertical/2.0) + 8, SquisheHorizontal, SquisheVertical);
    gb.display.setColor(WHITE,WHITE);
    gb.display.drawFastVLine(CPosX - (PPosX + SquisheHorizontal/2.0) + 6 + SquisheHorizontal, CPosY - (PPosY + SquisheVertical/2.0) + 10, 2);
    gb.display.drawFastVLine(CPosX - (PPosX + SquisheHorizontal/2.0) + 3 + SquisheHorizontal, CPosY - (PPosY + SquisheVertical/2.0) + 10, 2);
    gb.display.setColor(BLACK,WHITE);
  } else {
    gb.display.fillRect(CPosX - (PPosX + SquisheHorizontal/2.0) + 8, CPosY - (PPosY + SquisheVertical/2.0) + 8, SquisheHorizontal, SquisheVertical);
    gb.display.setColor(WHITE,WHITE);
    gb.display.drawFastVLine(CPosX - (PPosX + SquisheHorizontal/2.0) + 9, CPosY - (PPosY + SquisheVertical/2.0) + 10, 2);
    gb.display.drawFastVLine(CPosX - (PPosX + SquisheHorizontal/2.0) + 12, CPosY - (PPosY + SquisheVertical/2.0) + 10, 2);
    gb.display.setColor(BLACK,WHITE);
  }
}

////////////////
////Camera V////
////////////////

void ClampCamera () { //Clamp the camera in the world
  if(MapWidth < 11) {
    CPosX = (LCDWIDTH - MapWidth*8)/2;
  } else {
    CPosX = ClampInt(-(8*MapWidth-LCDWidth),0,PPosX+(LCDWIDTH/2)-8);
  }
  CPosY = ClampInt(-(8*MapHeigth-LCDHeight),-8,PPosY+(LCDHEIGHT/2)-8);
}

int ClampInt (int minv, int maxv, int value) {
  if(value < minv)
    return minv;
  else if(value >= maxv)
    return maxv;
  else
    return value;
}

///////////////////
////Colliding V////
///////////////////

byte GetTPSer1X(byte i) {
  return pgm_read_byte(TeleporterSerie1X + i);
}

byte GetTPSer1Y(byte i) {
  return pgm_read_byte(TeleporterSerie1Y + i);
}

byte GetTPSer2X(byte i) {
  return pgm_read_byte(TeleporterSerie2X + i);
}

byte GetTPSer2Y(byte i) {
  return pgm_read_byte(TeleporterSerie2Y + i);
}

int PixelInCollider (byte PIMX, byte PIMY, byte PICX, byte PICY) { //PIM = Pos in map (0-255), PIC = Pos in cube (0-8)
  
  byte GlobalColiderType = coltype[getTile(PIMX,PIMY)];
  
  byte x1 = 0;
  byte y1 = 0;
  byte x2 = 0;
  byte y2 = 0;
  
  byte Value = 0;
  switch (GlobalColiderType) {
      case 0:
        Value = 1;
        break;
      case 1:
        Value = 2;
        break;
      case 2:
        x1 = 1;
        y1 = 0;
        x2 = 6;
        y2 = 3;
        break;
      case 3:
        //Teleporter0
        for(int a = 0; a < TeleporterCount; a++) {
          if(GetTPSer1X(a) == PIMX && GetTPSer1Y(a) == PIMY) {
            //Found = true;
            PPosX = -((float)GetTPSer2X(a)*8) + 13;
            PPosY = -((float)GetTPSer2Y(a)*8) + 4; //Reverse
            VelocityX = 63;
            VelocityY = 40;
          }
          if(GetTPSer2X(a) == PIMX && GetTPSer2Y(a) == PIMY) {
            //Found = true;
            PPosX = -((float)GetTPSer1X(a)*8) + 13;
            PPosY = -((float)GetTPSer1Y(a)*8) + 4;
            VelocityX = 63;
            VelocityY = 40;
          }
        }
        break;
      case 4:
       //Teleporter0
        for(int a = 0; a < TeleporterCount; a++) {
          if(GetTPSer1X(a) == PIMX && GetTPSer1Y(a) == PIMY) {
            //Found = true;
            PPosX = -((float)GetTPSer2X(a)*8) - 5;
            PPosY = -((float)GetTPSer2Y(a)*8) + 4; //Reverse
            VelocityX = -63;
            AC = true;
            VelocityY = 40;
          }
          if(GetTPSer2X(a) == PIMX && GetTPSer2Y(a) == PIMY) {
            //Found = true;
            PPosX = -((float)GetTPSer1X(a)*8) - 5;
            PPosY = -((float)GetTPSer1Y(a)*8) + 4;
            VelocityX = -63;
            AC = true;
            VelocityY = 40;
          }
        }
        break;
      case 5:
        x1 = 0;
        y1 = 0;
        x2 = 4;
        y2 = 8;
        break;
      case 6:
        if(PistonPressed) {
          Value = 2;
        } else {
          Value = 1;
        }
        break;
      case 7:
        //That's the key
        Value = 1;
        for(int a = 0; a < KeyCount; a++) {
          if(KeyX[a] == PIMX && KeyY[a] == PIMY) {
            if(!KeyGot[a]) {
              sfx(4, 1);
              KeysGot++;
              KeyGot[a] = true;
              break;
            }
          }
        }
        break;
      case 8:
        //That's the locker
        if(KeysGot > 0) {
          for(int a = 0; a < LockerCount; a++) {
            if(LockerX[a] == PIMX && LockerY[a] == PIMY) {
              if(LockerGot[a] == false) {
                sfx(2, 1);
                LockerGot[a] = true;
                KeysGot--;
                break;
              }
            }
          }
        }
        for(int a = 0; a < LockerCount; a++) {
          if(LockerX[a] == PIMX && LockerY[a] == PIMY) {
            if(LockerGot[a]) {
              Value = 1;
            } else {
              Value = 2;
            }
            break;
          } else {
            Value = 2;
          }
        }
        break;
      case 9:
        x1 = 0;
        y1 = 0;
        x2 = 7;
        y2 = 7;
        PistonPressed = true;
        break;
      case 10:
        x1 = 2;
        y1 = 5;
        x2 = 7;
        y2 = 7;
        if(7-PICX >= x1 && 7-PICX <= x2 && 7-PICY>= y1 && 7-PICY<= y2) {
          sfx(7, 1);
          Die();
        }
        break;
      case 11:
        sfx(5, 2);
        UnlockNext();
        break;
      case 12:
        x1 = 0;
        y1 = 0;
        x2 = 7;
        y2 = 4;
        if(7-PICX >= x1 && 7-PICX <= x2 && 7-PICY>= y1 && 7-PICY<= y2) {
          VelocityX = 30;
          ACT = 0;
        }
        break;
      case 13:
        x1 = 0;
        y1 = 0;
        x2 = 7;
        y2 = 4;
        if(7-PICX >= x1 && 7-PICX <= x2 && 7-PICY>= y1 && 7-PICY<= y2) {
          VelocityY = -30;
          ACT = 1;
        }
        break;
      case 14:
        x1 = 0;
        y1 = 3;
        x2 = 7;
        y2 = 7;
        break;
      case 15:
        x1 = 0;
        y1 = 0;
        x2 = 7;
        y2 = 7;
        VelocityY = 110;
        ACT = 2;
        break;
      case 16:
        x1 = 0;
        y1 = 0;
        x2 = 7;
        y2 = 7;
        PistonPressed = false;
        break;
      case 17:
        if(!PistonPressed) {
          Value = 2;
        } else {
          Value = 1;
        }
        break;
      case 18:
        LoadCineEnd();
        break;
      default:
        Value = 1;
        break;
  }

  if(Value == 0) {
    if(7-PICX >= x1 && 7-PICX <= x2 && 7-PICY>= y1 && 7-PICY<= y2) {
      Value = 2;
    } else {
      Value = 1;
    }
  }
  return Value - 1;
}

void CheckForCollider () { //Based on current velocity        //Need improvement to support BigWhiteBox
  if(VelocityX != 0) {
  byte ColX[8];
  if(VelocityX > 0) {
      for(int xd = 0; xd < 8; xd++) { //yd = Y Down
          ColX[xd] = PixelInCollider( (-(floor((PPosX + 5 + 0) / 8.0))) , (-(floor((PPosY - 3 + xd) / 8.0))) , -(-(PPosX + 5/*e5<4*/ + 0) - (-(floor((PPosX + 5/*e5<4*/ + 0) / 8.0)))*8) , -(-(PPosY - 3 + xd) - (-(floor((PPosY - 3 + xd) / 8.0)))*8) );
      }
      for(int xd = 0; xd < 8; xd++) {
         if(ColX[xd] == 1) {
           //gb.popup(F("A wall has been detected"), 20);
           VelocityX = 0;
           GroundedLeft = true;
         }
       }
     }
     
     if(VelocityX < 0) {
      for(int xd = 0; xd < 8; xd++) { //yd = Y Down
         ColX[xd] = PixelInCollider( (-(floor((PPosX - 4 + 0) / 8.0))) , (-(floor((PPosY - 3 + xd) / 8.0))) , -(-(PPosX - 4 + 0) - (-(floor((PPosX - 4 + 0) / 8.0)))*8) , -(-(PPosY - 3 + xd) - (-(floor((PPosY - 3 + xd) / 8.0)))*8) );
       }
     }
     for(int xd = 0; xd < 8; xd++) {
       if(ColX[xd] == 1) {
         //gb.popup(F("A wall has been detected"), 20);
         VelocityX = 0;
         GroundedRight = true;
       }
     }
   }

    
    //working stuff V

    
   if(VelocityY != 0) {
     byte ColY[8];
     if(VelocityY > 0) {
        for(int yd = 0; yd < 8; yd++) { //yd = Y Down
          ColY[yd] = PixelInCollider((-(floor((PPosX - 3 + yd) / 8.0))) , (-(floor((PPosY + 5 + 0) / 8.0))) , -(-(PPosX - 3 + yd) - (-(floor((PPosX - 3 + yd) / 8.0)))*8) , -(-(PPosY + 5/*e5*/ + 0) - (-(floor((PPosY + 5/*e5*/ + 0) / 8.0)))*8));
        }
        for(int yd = 0; yd < 8; yd++) {
          if(ColY[yd] == 1) {
            VelocityY = 0;
          }
        }
      }
      if(VelocityY < 0) {
        for(int yd = 0; yd < 8; yd++) { //yd = Y Down
          ColY[yd] = PixelInCollider((-(floor((PPosX - 3 + yd) / 8.0))) , (-(floor((PPosY - 4 + 0) / 8.0))) , -(-(PPosX - 3/*e3*/ + yd) - (-(floor((PPosX - 3/*e3*/ + yd) / 8.0)))*8) , -(-(PPosY - 4/*e4*/ + 0) - (-(floor((PPosY - 4/*e4*/ + 0) / 8.0)))*8));
        }
        for(int yd = 0; yd < 8; yd++) {
          if(ColY[yd] == 1) {
            VelocityY = 0;
            GroundedDown = true;
          }
        }
      }
    }

    byte ColG[6];

    for(int yd = 0; yd < 6; yd++) { //yd = Y Down
      ColG[yd] = PixelInCollider((-(floor((PPosX - 2 + yd) / 8.0))) , (-(floor((PPosY - 3 + 0) / 8.0))) , -(-(PPosX - 2/*e3*/ + yd) - (-(floor((PPosX - 2/*e3*/ + yd) / 8.0)))*8) , -(-(PPosY - 3/*e4*/ + 0) - (-(floor((PPosY - 3/*e4*/ + 0) / 8.0)))*8));
    }
    for(int yd = 0; yd < 6; yd++) {
      if(ColG[yd] == 1) {
        VelocityY = 30;
      }
    }
}

//////////////////////
////PreparingMap V////
//////////////////////

void PrepareMap () {
  //PPosX = (8*SpawnCoordX[CurrentLoadedMap]);
  //PPosY = (8*SpawnCoordY[CurrentLoadedMap]);
  PPosX = -(8*pgm_read_byte(SpawnCoordX + CurrentLoadedMap)) + 4;                      //Convert Spawnpos to Worldpos
  PPosY = -(8*pgm_read_byte(SpawnCoordY + CurrentLoadedMap)) + 4;
  
  VelocityX = 0;
  VelocityY = 0;
  PistonPressed = false;
  IsPlaying = true;
  MapWidth = pgm_read_byte(MapSizeX + CurrentLoadedMap);
  MapHeigth = pgm_read_byte(MapSizeY + CurrentLoadedMap);

  KeysGot = 0;
  
  byte EnnemieC = 0;
  KeyCount = 0;
  LockerCount = 0;
  
  for(byte x = 0; x < MapWidth; x++) {                                  //Draw maps
    for(byte y = 0; y < MapHeigth; y++) {
      if(getTile(x,y)==9) {
        EnnemieC++;
      }
      if(getTile(x,y) == 19) {
        KeyCount++;
      }
      if(getTile(x,y) == 20) {
        LockerCount++;
      }
    }
  }

  EnnemieCount = EnnemieC;
  
  ennemieArray = new EnnemieAI[EnnemieCount];
  
  KeyX = new byte[KeyCount];
  KeyY = new byte[KeyCount];
  KeyGot = new bool[KeyCount];
  for(int q = 0; q < KeyCount; q++) {
    KeyGot[q] = false;
  }

  LockerX = new byte[LockerCount];
  LockerY = new byte[LockerCount];
  LockerGot = new bool[LockerCount];
  for(int q = 0; q < LockerCount; q++) {
    LockerGot[q] = false;
  }
  
  int i = 0;
  int a = 0;
  int b = 0;
  for(byte x = 0; x < MapWidth; x++) {                                  //Draw maps
    for(byte y = 0; y < MapHeigth; y++) {
      if(getTile(x,y)==9) {
        ennemieArray[i].DefinePos(x*8+0,y*8+0);
        i++;
      }
      if(getTile(x,y) == 19) {
        KeyX[a] = x;
        KeyY[a] = y;
        a++;
      }
      if(getTile(x,y) == 20) {
        LockerX[b] = x;
        LockerY[b] = y;
        b++;
      }
    }
  }

  if(CurrentLoadedMap == 0) {
    LoadCineStart();
  }
}

//////////////////
////Updating V////
//////////////////

boolean inRay (byte Min, byte Max, byte Value) {
  return Value <= Max && Value >= Min;
}

boolean inRange (byte r, byte v) {
  return inRay(r,r+8,v+2) || inRay(r,r+8,v+6);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(gb.update()) {

      if(IsPlaying) {
        GroundedDown = false;
      GroundedRight = false;
      GroundedLeft = false;

      if(last == false && GroundedDown == true) {
        sfx(3, 3);
      }

      last = GroundedDown;

      if(!(gb.buttons.timeHeld(BTN_RIGHT) > 0) && !(gb.buttons.timeHeld(BTN_LEFT) > 0)) {
        VelocityX = VelocityX * 0.6f;
      }

      if(VelocityY - 3 > -127) {
        VelocityY = VelocityY - 3;
      } else {
        VelocityY = -127;
      }

      if(gb.buttons.timeHeld(BTN_RIGHT) > 0) {
        if(VelocityX - 4 > -30) {
          VelocityX -= 4;
        }
        GoingRight = true;
      }
      if(gb.buttons.timeHeld(BTN_LEFT) > 0) {
        if(VelocityX + 4 < 30) {
          VelocityX += 4;
        }
        GoingRight = false;
      }

      ACT = 3;

      CheckForCollider();

      if(gb.buttons.pressed(BTN_A) && GroundedDown) {
        sfx(0, 0);
        VelocityY = 66;
      }

      if(gb.buttons.pressed(BTN_A) && GroundedRight && !GroundedDown) { 
        VelocityX = 60;
        VelocityY = 56;
        sfx(1, 0);
      }

      if(gb.buttons.pressed(BTN_A) && GroundedLeft && !GroundedDown) { 
        VelocityX = -60;
        VelocityY = 56;
        sfx(1, 0);
      }

      if(gb.buttons.pressed(BTN_C)) { 
        IsPlaying = false;
        SelectMap();
      }

      if(AC == true) {
        VelocityX = -63;
        AC = false;
      }
      if(ACT == 0) {
        VelocityX = -23;
      } else if(ACT == 1){
        VelocityX = 23;
      } else if (ACT == 2 && gb.buttons.pressed(BTN_A)) {
        VelocityY = 110;
      }
      
      PPosX += VelocityX / 127.0F * 3;
      PPosY += VelocityY / 127.0F * 3;

      if(VelocityY > 0) {
        //ex: 62 to 0 is flat to normal
        if(VelocityY > 62) {
          SquisheVertical = 5;
          SquisheHorizontal = 10;
        } else {
          SquisheVertical = 8+(VelocityY*BouncyMath2);
          SquisheHorizontal = 8+(VelocityY*BouncyMath1);
        }
      }
      if(VelocityY < 0) {
        //ex: 0 to -127 is normal to verticaly flat
        SquisheVertical = 8+(VelocityY*BouncyMath3);
          SquisheHorizontal = 8+(VelocityY*BouncyMath4);
      }
      if(VelocityY == 0) {
        SquisheVertical = 8;
        SquisheHorizontal = 8;
      }

      DrawPlayer();
      ClampCamera();

      for(byte c = 0; c < EnnemieCount; c++) {
        ennemieArray[c].update();
        if(ennemieArray[c].GetDirection()) {
          gb.display.drawBitmap(CPosX + ennemieArray[c].GetPosX() - 4, CPosY + ennemieArray[c].GetPosY() - 4, EnnemieSprite);
        } else {
          gb.display.drawBitmap(CPosX + ennemieArray[c].GetPosX() - 4, CPosY + ennemieArray[c].GetPosY() - 4, EnnemieSprite1);
        }
        if(inRange(ennemieArray[c].GetPosX() - 8, -PPosX) && inRange(ennemieArray[c].GetPosY() - 8, -PPosY)) {
          sfx(7, 1);
          Die();
        }
      }

      /*gb.display.setColor(GRAY, WHITE);
      for(byte x = 0; x < MapWidth; x++) {                                  //Draw maps
        for(byte y = 0; y < MapHeigth; y++) {
          if((x+y*MapWidth) % 2 == 0) {
            gb.display.drawBitmap(CPosX*(float)0.7 + x*8, CPosY*(float)0.7 + y*8, Back0);
          } else {
            gb.display.drawBitmap(CPosX*(float)0.7 + x*8, CPosY*(float)0.7 + y*8, Back1);
          }
        }
      }

      gb.display.setColor(BLACK, WHITE);*/
      for(byte x = 0; x < MapWidth; x++) {                                  //Draw maps
        for(byte y = 0; y < MapHeigth; y++) {
          if(getTile(x,y) == 15) {
            if(PistonPressed) {
              gb.display.drawBitmap(CPosX + x*8, CPosY + y*8, sprites[15]);
            } else {
              gb.display.drawBitmap(CPosX + x*8, CPosY + y*8, Missing);
            }
          } else if(getTile(x,y) == 19) {
            for(int a = 0; a < KeyCount; a++) {
              if(KeyX[a] == x && KeyY[a] == y) {
                if(KeyGot[a] == true) {
                  gb.display.drawBitmap(CPosX + x*8, CPosY + y*8, Empty);
                } else {
                  gb.display.drawBitmap(CPosX + x*8, CPosY + y*8, sprites[19]);
                }
                break;
              } else {
                if(a >= KeyCount) {
                  gb.display.drawBitmap(CPosX + x*8, CPosY + y*8, sprites[19]);
                }
              }
            }
          } else if(getTile(x,y) == 20) {
            for(int a = 0; a < LockerCount; a++) {
              if(LockerX[a] == x && LockerY[a] == y) {
                if(LockerGot[a]) {
                  gb.display.drawBitmap(CPosX + x*8, CPosY + y*8, Empty);
                } else {
                  gb.display.drawBitmap(CPosX + x*8, CPosY + y*8, sprites[20]);
                }
                break;
              } else {
                if(a >= LockerCount) {
                  gb.display.drawBitmap(CPosX + x*8, CPosY + y*8, sprites[20]);
                }
              }
            }
          } else if(getTile(x,y) == 28) {
            if(!PistonPressed) {
              gb.display.drawBitmap(CPosX + x*8, CPosY + y*8, sprites[15]);
            } else {
              gb.display.drawBitmap(CPosX + x*8, CPosY + y*8, Missing);
            }
          } else {
            gb.display.drawBitmap(CPosX + x*8, CPosY + y*8, sprites[getTile(x,y)]);
          }
        }
      }
      
      } else {
          if(Mode == 0) {
            SelectMap();
          } else if(Mode == 1) {
            for(byte x = 0; x < 3; x++) {
              for(byte y = 0; y < 3; y++) {
                if(getPreviewTile(x,y) == 9) {
                  gb.display.drawBitmap(30 + x*8, 12 + y*8, EnnemieSprite);
                } else {
                  gb.display.drawBitmap(30 + x*8, 12 + y*8, sprites[getPreviewTile(x,y)]);
                }
              }
            }
            if(MapCursor <= LevelsUnlock) {
              gb.display.drawBitmap(0,0,EUnLocked);
              if(gb.buttons.pressed(BTN_A)) {
                CurrentLoadedMap = MapCursor;
                IsPlaying = true;
                PrepareMap();
              }
            } else {
              gb.display.drawBitmap(0,0,ELocked);
            }
            if(MapCursor - 1 >= 0) {
              gb.display.drawBitmap(0,20,EArrow2);
              if(gb.buttons.pressed(BTN_LEFT)) {
                sfx(6, 2);
                MapCursor--;
              }
            }
            if(MapCursor + 1 < NbrOfLevel) {
              gb.display.drawBitmap(76,20,EArrow1);
              if(gb.buttons.pressed(BTN_RIGHT)) {
                sfx(6, 2);
                MapCursor++;
              }
            }
            if(gb.buttons.pressed(BTN_C)) { 
              IsPlaying = false;
              gb.begin();
              gb.titleScreen(F("v1.0"),Logo);
              gb.battery.show = false;
            }
          } else if(Mode == 2) {

            if(Scroll < 13) {
              Scroll++;
            }
            gb.display.fontSize = 2;

            gb.display.cursorX = 2;
            gb.display.cursorY = (-10+Scroll);

            gb.display.print(F("Blackout!"));

            gb.display.fontSize = 1;

            gb.display.cursorX = 2;
            gb.display.cursorY = (18+(Scroll*0.5f));

            gb.display.print(F("Press A to continue"));
            if(gb.buttons.pressed(BTN_A)) {
               SelectMap();
            }
         } else if(Mode == 3) {
          if(Scroll < 13) {
              Scroll++;
            }
            gb.display.fontSize = 2;

            gb.display.cursorX = 2;
            gb.display.cursorY = (-10+Scroll);

            gb.display.print(F("Good Job!"));

            gb.display.fontSize = 1;

            gb.display.cursorX = 2;
            gb.display.cursorY = (18+(Scroll*0.5f));

            gb.display.print(F("Press A to continue"));
            if(gb.buttons.pressed(BTN_A)) {
               SelectMap();
            }
         } else if(Mode == 4) {
            if(gb.buttons.pressed(BTN_A)) {
              IsPlaying = true;
              Frame = 0;
            }
            //Start - WhitePotion, Potion, Brick, EnnemieSprite, PlayerSprite, End, ExclPoint
            if(Frame < 45) {
              //Potion is falling and the ennemie is normal
              gb.display.drawBitmap((LCDWIDTH/2-4)+CamX,((Frame)-8+CamY)-8, WhitePotion); //Potion
              gb.display.drawBitmap((LCDWIDTH/2-4)+CamX,(LCDHEIGHT-8+CamY)-8, PlayerSprite); //Ennemie
              for(int i = 0; i < 14; i++) { //Ground
                if(i==0||i==13) {
                  gb.display.drawBitmap(-24+(i*8)+CamX,LCDHEIGHT-8, SharpBrick);
                } else {
                  gb.display.drawBitmap(-24+(i*8)+CamX,LCDHEIGHT-8, Brick);
                }
              }
            } else if(Frame < 80){
              //Ennemie is White, Cam scroll to the player, player has ExclamationPoint
              gb.display.drawBitmap((LCDWIDTH/2-4)+CamX,(LCDHEIGHT-8+CamY)-8, EnnemieSprite); //Ennemie
              gb.display.drawBitmap(-8+CamX,(LCDHEIGHT-8+CamY)-8,PlayerSprite); //Player
              gb.display.drawBitmap(-8+CamX+4,(LCDHEIGHT-8+CamY - 9)-8,ExclPoint); //ExclamationPoint
              for(int i = 0; i < 14; i++) { //Ground
                if(i==0||i==13) {
                  gb.display.drawBitmap(-24+(i*8)+CamX,LCDHEIGHT-8, SharpBrick);
                } else {
                  gb.display.drawBitmap(-24+(i*8)+CamX,LCDHEIGHT-8, Brick);
                }
              }
              gb.display.drawBitmap(-24-16+CamX,LCDHEIGHT-8,End); //Display Water
              gb.display.drawBitmap(-24-8+CamX,LCDHEIGHT-8,End); //Display Water
              CamX++;
            } else if(Frame < 110) {
              //Cam stop scroll, Player turn his big fat square face (replace face by something else) wait few frames
              gb.display.drawBitmap((LCDWIDTH/2-4)+CamX,(LCDHEIGHT-8+CamY)-8, EnnemieSprite); //Ennemie
              gb.display.drawBitmap(-8+CamX,(LCDHEIGHT-8+CamY)-8,PlayerSprite, NOROT, FLIPH); //Player
              for(int i = 0; i < 14; i++) { //Ground
                if(i==0||i==13) {
                  gb.display.drawBitmap(-24+(i*8)+CamX,LCDHEIGHT-8, SharpBrick);
                } else {
                  gb.display.drawBitmap(-24+(i*8)+CamX,LCDHEIGHT-8, Brick);
                }
              }
              gb.display.drawBitmap(-24-16+CamX,LCDHEIGHT-8,End); //Display Water
              gb.display.drawBitmap(-24-8+CamX,LCDHEIGHT-8,End); //Display Water
            } else {
              //Jumpin'n'dat water.
              gb.display.drawBitmap((LCDWIDTH/2-4)+CamX,(LCDHEIGHT-8+CamY)-8, EnnemieSprite); //Ennemie
              gb.display.drawBitmap(-8+CamX - (int)((Frame - 110)/(float)(140-110)*30),(LCDHEIGHT-8+CamY)-8 + (int)-(cos( ( (( ((float)Frame - 110)/((float)140-110))*180-90 )*(float)/*57.2958f*/0.0174533))*(float)16),PlayerSprite, NOROT, FLIPH); //Player
              for(int i = 0; i < 14; i++) { //Ground
                if(i==0||i==13) {
                  gb.display.drawBitmap(-24+(i*8)+CamX,LCDHEIGHT-8, SharpBrick);
                } else {
                  gb.display.drawBitmap(-24+(i*8)+CamX,LCDHEIGHT-8, Brick);
                }
              }
              gb.display.drawBitmap(-24-16+CamX,LCDHEIGHT-8,End); //Display Water (For the one who want to know why "Water" end a level? It's a one way teleporter!)
              gb.display.drawBitmap(-24-8+CamX,LCDHEIGHT-8,End); //Display Water
            }
            
            if(Frame >= 150) {
              IsPlaying = true;
              Frame = 0;
            }
            Frame++;
          } else if(Mode == 5) {
            //End  - Potion, Table, EnnemieSprite(Reverse), PlayerSprite
            if(Frame < 50) {
              for(int i = 0; i < 11; i++) {
                if(i==0||i==10) {
                  gb.display.drawBitmap(i*8+CamX,LCDHEIGHT-8+CamY,SharpBrick);
                } else {
                  gb.display.drawBitmap(i*8+CamX,LCDHEIGHT-8+CamY,Brick);
                }
              }
              
              gb.display.drawBitmap(38+CamX,LCDHEIGHT-16+CamY,Table);
              
              if(Frame < 25) {
                gb.display.drawBitmap(38+CamX,LCDHEIGHT-24+CamY,Potion);
              } else {
                gb.display.drawBitmap(38+CamX+((Frame-25)*(float)0.2),LCDHEIGHT-24+CamY-((Frame-25)*(float)0.1),Potion);
              }

              gb.display.drawBitmap(26+CamX + (Frame*(float)0.2),LCDHEIGHT-16+CamY - (Frame*(float)0.2),PlayerSprite);
              gb.display.drawBitmap(50+CamX,LCDHEIGHT-16+CamY,EnnemieSprite, NOROT, FLIPH);
            } else if(Frame < 90) {
              for(int i = 0; i < 11; i++) {
                if(i==0||i==10) {
                  gb.display.drawBitmap(i*8+CamX,LCDHEIGHT-8+CamY,SharpBrick);
                } else {
                  gb.display.drawBitmap(i*8+CamX,LCDHEIGHT-8+CamY,Brick);
                }
              }
              
              gb.display.drawBitmap(38+CamX,LCDHEIGHT-16+CamY,Table);
              
              gb.display.drawBitmap(48+CamX+((Frame-90)*(float)0.075),LCDHEIGHT-16/*22*/+CamY-((Frame-90)*(float)-0.2),Potion);

              gb.display.drawBitmap(26+CamX + ((Frame-90)*(float)-0.2),LCDHEIGHT-14/*22*/+CamY - ((Frame-90)*(float)-0.2),PlayerSprite);
              gb.display.drawBitmap(50+CamX,LCDHEIGHT-16+CamY,EnnemieSprite, NOROT, FLIPH);
            } else if(Frame >= 90 && Frame < 94) {
              gb.display.setColor(BLACK);
              gb.display.setColor(INVERT);
              gb.display.fillRect(0,0,84,48);
              for(int i = 0; i < 11; i++) {
                if(i==0||i==10) {
                  gb.display.drawBitmap(i*8+CamX,LCDHEIGHT-8+CamY,SharpBrick);
                } else {
                  gb.display.drawBitmap(i*8+CamX,LCDHEIGHT-8+CamY,Brick);
                }
              }
              gb.display.drawBitmap(38+CamX,LCDHEIGHT-16+CamY,Table);
              gb.display.drawBitmap(26+CamX,LCDHEIGHT-16+CamY,PlayerSprite);
              gb.display.drawBitmap(50+CamX,LCDHEIGHT-16+CamY,EnnemieSprite, NOROT, FLIPH);
              gb.display.setColor(BLACK);
            } else {
              for(int i = 0; i < 11; i++) {
                if(i==0||i==10) {
                  gb.display.drawBitmap(i*8+CamX,LCDHEIGHT-8+CamY,SharpBrick);
                } else {
                  gb.display.drawBitmap(i*8+CamX,LCDHEIGHT-8+CamY,Brick);
                }
              }
              gb.display.drawBitmap(38+CamX,LCDHEIGHT-16+CamY,Table);
              gb.display.drawBitmap(26+CamX,LCDHEIGHT-16+CamY,PlayerSprite);
              gb.display.drawBitmap(50+CamX,LCDHEIGHT-16+CamY, PlayerSprite, NOROT, FLIPH);

              gb.display.fontSize = 2;

              gb.display.cursorX = 2;
              gb.display.cursorY = (-10+13);

              gb.display.print(F("The end."));
            }
            
            if(Frame >= 500) {
              Frame = 0;
              SelectMap();
            }
            Frame+=3;
         }
      }
   }
}

void LoadCineStart() {
  Frame = 0;
  CamX = 0;
  CamY = 0;
  Mode = 4;
  IsPlaying = false;
}

void LoadCineEnd() {
  Frame = 0;
  CamX = 0;
  CamY = 0;
  Mode = 5;
  IsPlaying = false;
}

void SelectMap () {
  IsPlaying = false;
  Mode = 1;
}

void Die () {
  IsPlaying = false;
  Mode = 2;
  Scroll = 0;
}

void UnlockNext () {
  if(CurrentLoadedMap == LevelsUnlock) {
    LevelsUnlock++;
  }
  EEPROM.update(EEPROM_SAVE_START + 0, LevelsUnlock);
  Mode = 3;
  IsPlaying = false;
}
