//----------------------------------------------------------------------------
const byte PROGMEM gamelogo[]=
{
  64,26,
 B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,
 B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
 B00111011,B10011101,B11011100,B01110001,B11110001,B11111011,B11100001,B11110000,
 B00111011,B10011101,B11011100,B11111001,B11111001,B11111011,B11110011,B11111000,
 B00111011,B10011101,B11011100,B11111001,B11111001,B11111011,B11111011,B11111000,
 B00111011,B11011101,B11011101,B11111101,B11111101,B11000011,B10111011,B10111000,
 B00111011,B11011101,B11011101,B11111101,B11011101,B11000011,B10111011,B10111000,
 B00111011,B11011101,B11011101,B11011101,B11011101,B11000011,B10111011,B10000000,
 B00111011,B11111101,B11011101,B11011101,B11011101,B11110011,B10111011,B11000000,
 B00111011,B11111101,B11011101,B11011101,B11011101,B11110011,B11110001,B11100000,
 B00111011,B11111101,B11011101,B11111101,B11011101,B11110011,B11100000,B11110000,
 B00111011,B11111101,B11011101,B11111101,B11011101,B11000011,B11110000,B01111000,
 B00111011,B10111101,B11011101,B11111101,B11011101,B11000011,B10111000,B00111000,
 B00111011,B10111101,B11011101,B11011101,B11011101,B11000011,B10111011,B10111000,
 B00111011,B10111101,B11111101,B11011101,B11011101,B11000011,B10111011,B10111000,
 B00111011,B10011100,B11111001,B11011101,B11111001,B11111011,B10111011,B11111000,
 B00111011,B10011100,B01110001,B11011101,B11111001,B11111011,B10111011,B11111000,
 B00111011,B10011100,B00100001,B11011101,B11110001,B11111011,B10111001,B11110000,
 B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
 B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,
 B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
 B10100110,B11000110,B01101010,B11101100,B11100110,B01101110,B01100110,B11001110,
 B10101010,B10101010,B10001010,B01001010,B10001010,B10101010,B10001010,B10101000,
 B10101010,B10101110,B11101010,B01001010,B11001010,B11101100,B10001110,B10101100,
 B01001010,B10101010,B00101010,B01001010,B10001010,B10101010,B10001010,B10101000,
 B01001110,B11001010,B11100100,B11101100,B11101110,B10101010,B11101010,B11001110,
};
//----------------------------------------------------------------------------
const byte PROGMEM invader[8][7] = {
  {7,5, B00111000,B11010110,B11111110,B00101000,B11000110},
  {7,5, B00111000,B11010110,B11111110,B01000100,B00101000},
  {7,5, B00010000,B10111010,B11010110,B01111100,B00101000},
  {7,5, B00111000,B01010100,B11111110,B10010010,B00101000},
  {7,5, B00111000,B01010100,B01111100,B01000100,B00101000},
  {7,5, B00111000,B01010100,B01111100,B00101000,B01010100},
  {7,5, B01010100,B10000010,B01010100,B10000010,B01010100},
  {7,5, B01010100,B10000010,B01010100,B10000010,B01010100},
};
//----------------------------------------------------------------------------
const byte PROGMEM playership[3][6] = {
  {7,4, B00010000,B01111100,B11111110,B11111110},
  {7,4, B10001010,B01000000,B00000100,B10010010},
  {7,4, B10010010,B00000100,B01000000,B10001010},
};
//----------------------------------------------------------------------------
const byte PROGMEM bunker[5][7] = {
  {8,5, B01111110,B11111111,B11111111,B11100111,B11000011},
  {8,5, B01111110,B11011011,B11111111,B10100101,B11000011},
  {8,5, B01101110,B11011011,B01110110,B10100101,B11000011},
  {8,5, B01100110,B11011001,B01010110,B10100101,B01000010},
  {8,5, B00100010,B10001001,B01010010,B10100101,B01000010},  
};
//----------------------------------------------------------------------------
const byte PROGMEM bomb[2][6] = {
  {2,4, B10000000,B01000000,B10000000,B01000000},
  {2,4, B01000000,B10000000,B01000000,B10000000},
};
//----------------------------------------------------------------------------
const byte PROGMEM saucer[2][10] = {
  {11,4, B00011111,B00000000,B01101010,B11000000,B11111111,B11100000,B01100100,B11000000},
  {11,4, B01011101,B11000000,B01010101,B01000000,B01010101,B01000000,B01011101,B11000000},
};
//----------------------------------------------------------------------------