// Wave, pitch, duration, arpeggio step duration, arpeggio step size
const int8_t sounds[][5] = {
  { 0, 20, 5, 1, 1 }, //Player launch
  { 0, 25, 5, 1, -1 }, //Enemy launch
  { 1, 10, 5, 1, -1 }, //Detonating enemy missile
  { 1, 10, 2, 0, 0 }, //Score pips
  { 1, 2, 10, 1, -1 }, //A city dies
  { 0, 20, 14, 3, -1 }, //Lose
};

void initSound(){
  gb.sound.command(CMD_VOLUME, 5, 0, 0);
  gb.sound.command(CMD_SLIDE, 0, 0, 0);
}

void playSound(uint8_t i){
  gb.sound.command(CMD_VOLUME, 5, 0, 0);
  gb.sound.command(CMD_SLIDE, 0, 0, 0);
  gb.sound.command(CMD_ARPEGGIO, sounds[i][3], sounds[i][4], 0); 
  gb.sound.command(CMD_INSTRUMENT, sounds[i][0], 0, 0);
  gb.sound.playNote(sounds[i][1], sounds[i][2], 0);

  //gb.sound.playNote(20, 5, 0);
}

