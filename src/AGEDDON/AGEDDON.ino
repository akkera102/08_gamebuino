#include <SPI.h>
//Use for PROGMEM
#include <avr/pgmspace.h>
#include <EEPROM.h>
#include "lib_Gamebuino.h"
Gamebuino gb;

const byte armageddon[] PROGMEM = {64,36,
B11111111,B11110000,B00000000,B00000000,B00000000,B00000000,B00001111,B11111111,
B11111100,B00001111,B11111111,B11111111,B11111111,B11111111,B11110000,B00111111,
B11100011,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11000111,
B10011111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111001,
B01110111,B10011110,B11101110,B11111001,B11000010,B01111001,B11110111,B10111010,
B01110111,B10101110,B01001110,B11110110,B11011110,B10111010,B11101011,B10011010,
B01101011,B10001110,B01001101,B01110111,B11000110,B11011011,B01011101,B10101010,
B01100011,B10101110,B10101100,B01110100,B11011110,B11011011,B01011101,B10110010,
B01101011,B10100110,B11101101,B01110110,B11011110,B10111010,B11101011,B10111010,
B01001001,B10110110,B11101001,B00111001,B11000010,B01111001,B11110111,B10111010,
B10111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111101,
B11011111,B11111111,B11100000,B11111111,B11111111,B00000111,B11111111,B11111011,
B11100011,B11111110,B00011111,B00011111,B11111000,B11111000,B01111111,B11000111,
B11111000,B11100001,B11111111,B11101111,B11110111,B11111111,B10000111,B00011111,
B11111110,B00001111,B11111111,B11110111,B11101111,B11111111,B11110000,B01111111,
B11111111,B11111111,B11111111,B11110111,B11101111,B11111111,B11111111,B11111111,
B11111111,B11111111,B11111111,B11111011,B11011111,B11111111,B11111111,B11111111,
B11111111,B11111111,B11111111,B11111011,B11011111,B11111111,B11111111,B11111111,
B11111111,B11111111,B11111110,B00000011,B11000000,B01111111,B11111111,B11111111,
B11111111,B11111111,B11100001,B11111011,B11011111,B10000111,B11111111,B11111111,
B11111111,B11111111,B11011111,B11110011,B11001111,B11111011,B11111111,B11111111,
B11111111,B11111111,B11011111,B11110011,B11001111,B11111011,B11111111,B11111111,
B11111111,B11111111,B11011111,B11110011,B11001111,B11111011,B11111111,B11111111,
B11111111,B11111111,B11100001,B11110011,B11001111,B10000111,B11111111,B11111111,
B11111111,B11111111,B11111110,B00010111,B11101000,B01111111,B11111111,B11111111,
B11111111,B11111111,B11111111,B11100000,B00000111,B11111111,B11111111,B11111111,
B11111111,B11111111,B11111111,B11110111,B11101111,B11111111,B11111111,B11111111,
B11111111,B11111111,B11111111,B11100111,B11100111,B11111111,B11111111,B11111111,
B11111111,B11111111,B11111111,B11100111,B11100111,B11111111,B11111111,B11111111,
B11111111,B11111111,B11111111,B11101111,B11110111,B11111111,B11111111,B11111111,
B11111111,B11111111,B11111111,B11101111,B11110111,B11111111,B11111111,B11111111,
B11111111,B11111111,B11111111,B11101111,B11110111,B11111111,B11111111,B11111111,
B11111111,B11111111,B11111111,B11011111,B11111011,B11111111,B11111111,B11111111,
B11111111,B11111111,B11111111,B10111111,B11111101,B11111111,B11111111,B11111111,
B11111111,B11111111,B11111110,B01111111,B11111110,B01111111,B11111111,B11111111,
B11111111,B11111111,B11111101,B11111111,B11111111,B10111111,B11111111,B11111111,
};

/*const byte city[] PROGMEM = {8,8,
B00000000,
B00000000,
B00000000,
B00000000,
B00100010,
B01110110,
B01111110,
B11111111,
};*/

const byte city[] PROGMEM = {8,8,
B00000000,
B00000000,
B00000000,
B00000000,
B01100100,
B01101110,
B01111111,
B11111111,
};

const byte deadcity[] PROGMEM = {8,8,
B00000000,
B00000000,
B00000000,
B00000000,
B00000000,
B00000000,
B00100000,
B01100010,
};

const byte launcher[] PROGMEM = {8,8,
B00011000,
B00011000,
B00011000,
B00100100,
B00100100,
B01000010,
B01000010,
B10000001,
};

const byte deadlauncher[] PROGMEM = {8,8,
B00000000,
B00000000,
B00000000,
B00000000,
B00000000,
B01100110,
B01011010,
B10000001,
};

#define SOUND_PLAUNCH 0
#define SOUND_ELAUNCH 1
#define SOUND_DETONATE 2
#define SOUND_SCORE 3
#define SOUND_DEAD 4
#define SOUND_LOSE 5

#define MODE_PREGAME 0
#define MODE_GAME 1 
#define MODE_LULL 2
#define MODE_DEAD 3
#define MODE_POSTDEAD 4

#define TARGET_SPEED 3

#define LAUNCHER_ONE 1
#define LAUNCHER_TWO 2
#define MAX_PMISSILES 10
#define PSPEED 4
#define PRADIUS 7
#define EXPAND 0
#define SHRINK 1

#define MAX_EMISSILES 10
#define MAX_CHANCE 50

// State variables
uint8_t mode = 0;
uint8_t counter = 0;
uint8_t flash = 0;
uint8_t stage = 0; //Maximum of 255 stages
uint32_t score = 0; //Score needs more space than other vars

uint8_t lullMissiles = 0;
uint8_t lullCities[8] = {0,0,0,0,0,0,0,0};

uint8_t cities[8] = {1,1,1,1,1,1,1,1}; //Whether the cities or launchers are alive

uint8_t targetX = 84/2;
uint8_t targetY = 48/2;
uint8_t pammo[2] = {10,10};
uint8_t pDests[MAX_PMISSILES][2] = {{100,100},{100,100},{100,100},{100,100},{100,100},{100,100},{100,100},{100,100},{100,100},{100,100}};
float pMissiles[MAX_PMISSILES][3] = {{100,100,0},{100,100,0},{100,100,0},{100,100,0},{100,100,0},{100,100,0},{100,100,0},{100,100,0},{100,100,0},{100,100,0}};
uint8_t pDetonations[MAX_PMISSILES][4] = {{100,100,0,0},{100,100,0,0},{100,100,0,0},{100,100,0,0},{100,100,0,0},{100,100,0,0},{100,100,0,0},{100,100,0,0},{100,100,0,0},{100,100,0,0}};

uint8_t etotal = 5;
uint8_t echance = 1;
float espeed = 0.2;
uint8_t eDests[MAX_EMISSILES] = {100,100,100,100,100,100,100,100,100,100};
float eMissiles[MAX_EMISSILES][4] = {{100,100,100,100},{100,100,100,100},{100,100,100,100},{100,100,100,100},{100,100,100,100},{100,100,100,100},{100,100,100,100},{100,100,100,100},{100,100,100,100},{100,100,100,100}};

void setup() {
  gb.begin();
  loadHighscores();
  gb.titleScreen(armageddon);
  gb.battery.show = false;
  gb.pickRandomSeed();
}

void nextStage(){
  stage++;
  //Reset cities (since we cleared them for the visual effect
  for( uint8_t i = 0; i < 8; i++ ){
    cities[i] = lullCities[i];
  }
  //Reset launchers
  cities[2] = 1;
  cities[5] = 1;
  //Reset missiles
  pammo[0] = 10;
  pammo[1] = 10;
  //Reset in-flight missiles
  for( uint8_t i = 0; i < 10; i++ ){
    pDests[i][0] = 100;
    pDetonations[i][0] = 100;
    eDests[i] = 100;
  }

  etotal = stage > 10 ? 20 : 10+stage; //Max of 20 missiles per stage, start at 10
  //echance = stage > 5 ? 5 : stage; //Max of 5/100 chance per frame
  espeed = stage > 18 ? 2 : 0.2+(stage*0.1); //Max speed of 2, up 0.1 per stage, starts at 0.2
}

void nextLull(){
  //Reset lullMissiles
  lullMissiles = 0;
  //Reset lullCities
  for( uint8_t i = 0; i < 8; i++ ){
    lullCities[i] = 0;
  }
}

void drawScore(){
  gb.display.cursorX = 84/2 - 4*3;
  gb.display.cursorY = 0;

  if( score < 100000 ){
    gb.display.print(0);
  }
  if( score < 10000 ){
    gb.display.print(0);
  }
  if( score < 1000 ){
    gb.display.print(0);
  }
  if( score < 100 ){
    gb.display.print(0);
  }
  if( score < 10 ){
    gb.display.print(0);
  }
  gb.display.print(score);
}

void drawTargets(){
  gb.display.drawFastHLine(targetX-1,targetY,3);
  gb.display.drawFastVLine(targetX,targetY-1,3);

  for(uint8_t i = 0; i < MAX_PMISSILES; i++){
    //Check for a valid destination without a current detonation
    if( pDests[i][0] <= 84 && pDetonations[i][0] > 84 ){
      gb.display.drawPixel(pDests[i][0], pDests[i][1]);
    }
  }
}

void drawCities(){
  //uint8_t alldead = 1;
  for(uint8_t i = 0; i < 8; i++){
      if( i == 2 || i == 5 ){
        if( cities[i] ){
          gb.display.drawBitmap(i*10+2,40,launcher);
        }else{
          gb.display.drawBitmap(i*10+2,40,deadlauncher);
        }
      }else{
        if( cities[i] ){
          //alldead = 0;
          gb.display.drawBitmap(i*10+2,40,city);
        }else{
          gb.display.drawBitmap(i*10+2,40,deadcity);
        }
      }
    }

    /*
    if( alldead ){
      gb.display.cursorX = 84/2 - 5*3;
      gb.display.cursorY = 48/2 - 5;
      gb.display.print("THE END");
    }
    */
}

void drawAmmo(){
  for( uint8_t i = 0; i < 2; i++ ){
    uint8_t xcoord = i == 0 ? 25 : 55;
    uint8_t ycoord = 47;
    if( cities[i*3+2] ){ //Is launcher alive?
      for( uint8_t j = 0; j < pammo[i]; j++ ){
        gb.display.drawPixel(xcoord,ycoord);
        if( xcoord % 2 == 0 ){
          xcoord--;
          ycoord--;
        }else{
          xcoord++;
        }
      }
    }
  }
}

void drawMissiles(){
  //Player Missiles
  for(uint8_t i = 0; i < MAX_PMISSILES; i++){
    //Check for a valid destination without a current detonation
    if( pDests[i][0] <= 84 && pDetonations[i][0] > 84 ){
      if(pMissiles[i][2] == LAUNCHER_ONE){
        gb.display.drawLine(25,40,pMissiles[i][0], pMissiles[i][1]);
      }else{
        gb.display.drawLine(56,40,pMissiles[i][0], pMissiles[i][1]);
      }
    }
  }

  //Enemy Missiles
  for(uint8_t i = 0; i < MAX_EMISSILES; i++){
    //Check for a valid destination
    if( eDests[i] <= 84 ){
        gb.display.drawLine(eMissiles[i][0],eMissiles[i][1],eMissiles[i][2], eMissiles[i][3]);
    }
  }
}

void drawDetonations(){
  for(uint8_t i = 0; i < MAX_PMISSILES; i++){
    if( pDetonations[i][0] <= 84 ){
      gb.display.drawCircle(pDetonations[i][0],pDetonations[i][1],pDetonations[i][2]);
    }
  }
}

void launchMissile(uint8_t launcher){
  //If there is no free slot in pDests, do not launch
  for(uint8_t i = 0; i < MAX_PMISSILES; i++){
    if( pDests[i][0] > 84 ){
      // Which launcher?             Alive?       Has ammo?
      if(launcher == LAUNCHER_ONE && cities[2] && pammo[0]){
        pDests[i][0] = targetX;
        pDests[i][1] = targetY;
        pMissiles[i][0] = 25; //X-coord of left launcher
        pMissiles[i][2] = LAUNCHER_ONE; //Launched from launcher one
        pammo[0]--;
        playSound(SOUND_PLAUNCH);
      }else if(launcher == LAUNCHER_TWO && cities[5] && pammo[1]){
        pDests[i][0] = targetX;
        pDests[i][1] = targetY;
        pMissiles[i][0] = 56; //X-coord of right launcher
        pMissiles[i][2] = LAUNCHER_TWO; //Launched from launcher two
        pammo[1]--;
        playSound(SOUND_PLAUNCH);
      }
      pMissiles[i][1] = 40; //Y-coord of both launchers
      break;
    }
  }
}

void tryLaunchEnemy(){
  uint8_t someActive = 0;

  if( etotal > 0 ){
  
    // Check whether there are any active missiles
    // If none are active, always spawn one to avoid
    // long pauses without any enemy missiles
    for(uint8_t i = 0; i < MAX_EMISSILES; i++){
      if( eDests[i] <= 84 ){
        someActive = 1;
        break;
      }
    }
    
    if( (!someActive || echance >= random(100)) ){ //echance of 100
      for(uint8_t i = 0; i < MAX_EMISSILES; i++){
        if( eDests[i] > 84 ){
          etotal--;
          eDests[i] = random(8); //Target one of the 6 cities or 2 launch sites
          eMissiles[i][0] = random(84); //Screen width
          eMissiles[i][1] = 0; //Top of screen
          eMissiles[i][2] = eMissiles[i][0]; //Start and end are same
          eMissiles[i][3] = 0; //Top of screen
          playSound(SOUND_ELAUNCH);
          break; //Only spawn one
        }
      }
    }

  }
}

void stepMissiles(){
  //Player Missiles
  for(uint8_t i = 0; i < MAX_PMISSILES; i++){
    //Check for a valid destination without a current detonation
    if( pDests[i][0] <= 84 && pDetonations[i][0] > 84 ){
      //If the missile is close enough to the destination, detonate
      if( abs( pDests[i][0] - pMissiles[i][0] ) < PSPEED && abs( pDests[i][1] - pMissiles[i][1] ) < PSPEED ){
        pDetonations[i][0] = pDests[i][0];
        pDetonations[i][1] = pDests[i][1];
        pDetonations[i][2] = 0; //Start detonation at radius of 0
        pDetonations[i][3] = EXPAND; //Detonation is increasing in size
      //Otherwise, keep moving towards destination
      }else{
        float dir = atan2(pDests[i][1]-pMissiles[i][1], pDests[i][0]-pMissiles[i][0]);
        pMissiles[i][0] += PSPEED * cos(dir);
        pMissiles[i][1] += PSPEED * sin(dir);
      }
    }
  }

  //Enemy Missiles
  for(uint8_t i = 0; i < MAX_EMISSILES; i++){
    //Check for a valid destination
    if( eDests[i] <= 84 ){
       //If enemy missile is close enough to the destination, detonate
      if( abs( (eDests[i]*10+6) - eMissiles[i][2] ) < PSPEED && abs( 44 - eMissiles[i][3] ) < PSPEED ){
        cities[eDests[i]] = 0; //Destroy city/launcher
        
        //If launcher, remove its ammo
        if( eDests[i] == 2 ){
          pammo[0] = 0;
        }
        if( eDests[i] == 5 ){
          pammo[1] = 0;
        }
        
        eDests[i] = 100; //Reset enemy missile
        playSound(SOUND_DEAD);
      //Otherwise, keep moving towards destination
      }else{
        float dir = atan2( 44-eMissiles[i][3], (eDests[i]*10+6)-eMissiles[i][2] );
        eMissiles[i][2] += espeed * cos(dir);
        eMissiles[i][3] += espeed * sin(dir);
      }
    }
  }
}

void stepDetonations(){
  if( counter%2 == 0 ){
    for(uint8_t i = 0; i < MAX_PMISSILES; i++){
      if( pDetonations[i][0] <= 84 ){
        if( pDetonations[i][3] == EXPAND ){ //If detonation is expanding
          if( pDetonations[i][2] < PRADIUS ){
            pDetonations[i][2]++;
          }else{
            pDetonations[i][3] = SHRINK; //Start shrinking
          }
        }
        //Check this now instead of using else because it may have just
        //set to SHRINK; immediately checking will remove a delay at full
        //size.  If a delay is desired, this if can be switched to an else.
        if( pDetonations[i][3] == SHRINK ){ //If detonation is shrinking
          if( pDetonations[i][2] > 0 ){
            pDetonations[i][2]--;
          }else{
            pDetonations[i][0] = 100; //Detonation is complete, remove it
            pDests[i][0] = 100; //Remove this destination
          }
        }
      }
    }
  }
}

void stepCollision(){
  //Player Detonations with Enemy Missiles
  for(uint8_t i = 0; i < MAX_PMISSILES; i++){
      if( pDetonations[i][0] <= 84 ){
        for(uint8_t j = 0; j < MAX_EMISSILES; j++){
          if( eDests[j] <= 84 && pDetonations[i][2] >= sqrt( (eMissiles[j][2]-pDetonations[i][0])*(eMissiles[j][2]-pDetonations[i][0]) + (eMissiles[j][3]-pDetonations[i][1])*(eMissiles[j][3]-pDetonations[i][1]) ) ){
            eDests[j] = 100; //Remove enemy missile
            score+=25;
            playSound(SOUND_DETONATE);
          }
        }
      }
  }
}


void checkMenu(){
  if( gb.buttons.pressed(BTN_C) ){
    gb.titleScreen(armageddon);
    gb.battery.show = false;
  }
}

void checkWin(){
  if( etotal == 0 ){
    for(uint8_t i = 0; i < MAX_EMISSILES; i++){
      //Check for a valid destination
      if( eDests[i] <= 84 ){
        return;
      }
    }
    //If we get here, all enemy missiles are destroyed and they have no more
    nextLull();
    mode = MODE_LULL;
  }
}

void checkLose(){
  for(uint8_t i = 0; i < 8; i++){
    if( i != 2 && i != 5 ){
      if( cities[i] ){
        return; // A city remains alive
      }
    }
  }
  //If we get here, all cities are dead.  It is the end of days.
  counter = 0;
  mode = MODE_DEAD;
  playSound(SOUND_LOSE);
}

void stepGame(){
  //Player input
  if( gb.buttons.pressed(BTN_A) ){
    launchMissile(LAUNCHER_ONE);
  }
  if( gb.buttons.pressed(BTN_B) ){
    launchMissile(LAUNCHER_TWO);
  }
  
  if( gb.buttons.repeat(BTN_LEFT,1) ){
    targetX = targetX-TARGET_SPEED > 0 ? targetX-TARGET_SPEED : 0;
  }
  if( gb.buttons.repeat(BTN_RIGHT,1) ){
    targetX = targetX+TARGET_SPEED < 84 ? targetX+TARGET_SPEED : 84;
  }
  if( gb.buttons.repeat(BTN_UP,1) ){
    targetY = targetY-TARGET_SPEED > 0 ? targetY-TARGET_SPEED : 0;
  }
  if( gb.buttons.repeat(BTN_DOWN,1) ){
    targetY = targetY+TARGET_SPEED < 48 ? targetY+TARGET_SPEED : 48;
  }

  //Game logic
  tryLaunchEnemy();
  stepMissiles();
  stepDetonations();
  stepCollision();

  //Drawing
  drawScore();
  drawTargets();
  drawCities();
  drawAmmo();
  drawMissiles();
  drawDetonations();

  checkWin();
  checkLose();
}

void drawLull(){
  uint8_t cityCount = 0;
  gb.display.cursorX = 84/2 - 4*6;
  gb.display.cursorY = 48/2 - 5*3;
  gb.display.print(F("BONUS POINTS"));

  gb.display.cursorX = 84/2 - 4*8;
  gb.display.cursorY += 5*2;
  gb.display.print(lullMissiles);

  for(uint8_t i = 0; i < lullMissiles; i++){
    gb.display.drawPixel(84/2 - 4*6 + i*2,48/2 - 3);
  }

  gb.display.cursorX = 84/2 - 4*8;
  gb.display.cursorY += 5*2;
  for( uint8_t i; i < 8; i++ ){
    if( lullCities[i] ) cityCount++;
  }
  gb.display.print(cityCount);

  for(uint8_t i = 0; i < cityCount; i++){
    gb.display.drawBitmap(84/2 - 4*6 + i*9,48/2+2, city);
  }

  drawScore();
  drawCities();
  drawAmmo();
}

void stepLull(){
  
  if( counter % 4 == 0 && (pammo[0] > 0 || pammo[1] > 0) ){
    lullMissiles++;
    score+=10;
    if( pammo[0] > 0 ) pammo[0]--;
    else pammo[1]--;
    playSound(SOUND_SCORE);
  }

  //If we have already iterated through the missiles
  if( counter % 8 == 0 && pammo[0] == 0 && pammo[1] == 0 ){
    for( uint8_t i = 0; i < 9; i++ ){
      if( i == 8 ){ //We have iterated through all live cities
        nextStage();
        mode = MODE_GAME;
        return;
      }
      //If not a launcher and the city is alive
      if( i != 2 && i != 5 && cities[i] != 0 ){
        lullCities[i] = 1;
        score+=100;
        cities[i] = 0;
        playSound(SOUND_SCORE);
        break;
      }
    }
  }

  drawLull();
}

void stepDead(){
  gb.display.cursorX = 84/2 - 5*3;
  gb.display.cursorY = 48/2 - 5;
  gb.display.print(F("THE END"));

  if( mode == MODE_DEAD && counter%20 == 0 ){
    mode = MODE_POSTDEAD;
  }else if( mode == MODE_POSTDEAD ){
    if( counter%8 == 0 ){
      flash ^= 255;
    }
    if( flash ){
      gb.display.cursorX = 84/2 - 5*3;
      gb.display.cursorY = 48 - 9;
      gb.display.print(F("PRESS \25"));

      if( isHighscore(score) ){
        gb.display.cursorX = 84/2 - 5*5 - 2;
        gb.display.cursorY = 48 - 15;
        gb.display.print(F("NEW HIGHSCORE"));
      }
    }

    if( gb.buttons.pressed(BTN_A) ){
      if( isHighscore(score) ){
        char tmp_name[11];
        gb.getDefaultName(tmp_name);
        gb.keyboard(tmp_name, 11);
        saveHighscore(score,tmp_name);
      }
      score = 0;
      mode = MODE_PREGAME;
    }
  }
}

void stepPregame(){
  drawHighscores();

  if( counter%8 == 0 ){
    flash ^= 255;
  }
  if( flash ){
    gb.display.cursorX = 84/2 - 5*3;
    gb.display.cursorY = 48 - 9;
    gb.display.print(F("PRESS \25"));
  }

  if( gb.buttons.pressed(BTN_A) ){
    stage = 255;
    for( uint8_t i = 0; i < 8; i++ ){
      lullCities[i] = 1;
    }
    nextStage(); //Reset to stage 0
    mode = MODE_GAME;
  }
}

void loop() {
  if(gb.update()){

    switch( mode ){
      case MODE_GAME:
        stepGame();
        break;

      case MODE_LULL:
        stepLull();
        break;

      case MODE_DEAD:
      case MODE_POSTDEAD:
        stepDead();
        break;

      case MODE_PREGAME:
        stepPregame();
        break;
    }

    checkMenu();

    counter++;
  }
}
