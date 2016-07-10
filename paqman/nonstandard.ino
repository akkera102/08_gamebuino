void newgame() {
  score=0;
  lives=3;
  gamelevel=0;
  gamestatus="newlevel"; 
}
//----------------------------------------------------------------------------
void newlevel() {
  for (int i = 0 ; i < 132 ; i++) { leveldots[i]=dotscreen[i]; }
  dotstoeat=244;
  fruitshape=gamelevel;
  if (fruitshape>7) { fruitshape=7; }
  gamestatus="newlife"; 
}
//----------------------------------------------------------------------------
void newlife() {
  powerpilltimer=0;
  yeahtimer=0;
  deadtimer=-1;
  paqmanx=13;
  paqmany=24;
  paqmanxadd=2;
  paqmanyadd=0;
  paqmanxdir=1;
  paqmanydir=0;
  paqmanghostxdir=1;
  paqmanghostydir=1;
  paqmanshape=12;
  paqmananimation=3;
  fruitvisible=0;
  waittime=30;
  ghostx[0]=13;
  ghosty[0]=12;
  ghostxadd[0]=1;
  ghostyadd[0]=0;
  ghostxdir[0]=1;
  ghostydir[0]=0;
  ghostx[1]=12;
  ghosty[1]=15;
  ghostxadd[1]=0;
  ghostyadd[1]=0;
  ghostxdir[1]=1;
  ghostydir[1]=0;
  ghostx[2]=13;
  ghosty[2]=15;
  ghostxadd[2]=2;
  ghostyadd[2]=0;
  ghostxdir[2]=1;
  ghostydir[2]=0;
  ghostx[3]=15;
  ghosty[3]=15;
  ghostxadd[3]=0;
  ghostyadd[3]=0;
  ghostxdir[3]=-1;
  ghostydir[3]=0;
  ghostmode="chase";
  ghostmodetime=1;
  gamestatus="running";
}
//----------------------------------------------------------------------------
void showscore() {
  if (screenyoffset==-5 or screenyoffset==56) {
    int i=0;
    if (screenyoffset==56) { i=43; }
    if (lives>1) { gb.display.drawBitmap(0,i,paqman[3]); }
    if (lives>2) { gb.display.drawBitmap(6,i,paqman[3]); }
    gb.display.cursorX=40-2*(score>9)-2*(score>99)-2*(score>999);
    gb.display.cursorY=i;
    gb.display.print(score);
    gb.display.cursorX=72;
    gb.display.print(gamelevel+1);
  }
}
//----------------------------------------------------------------------------
void nextlevelcheck() {
  if (yeahtimer>0) {
    yeahtimer=--yeahtimer;
    if (yeahtimer==0) {
      gamelevel=gamelevel+1;
      gamestatus="newlevel"; 
    } 
  }  
}
//----------------------------------------------------------------------------
void handledeath() {
  deadtimer=--deadtimer;
  if (deadtimer % 5 == 0) {
    gb.sound.playTick(); // play dead sound
  }
  int i = 7 - deadtimer / 10;
  gb.display.drawBitmap(paqmanx*3+paqmanxadd-1,paqmany*3+paqmanyadd-screenyoffset-1,paqman[12+i]);
  if (deadtimer==0) {
    deadtimer=-1;
    lives=--lives;  
    if (lives == 0) {
      gamestatus = "gameover";
    } else {
      gamestatus = "newlife";          
    }          
  }
}
//----------------------------------------------------------------------------

