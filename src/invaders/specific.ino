//----------------------------------------------------------------------------
void checkbuttons() {
  if (gb.buttons.repeat(BTN_LEFT,1) and shipx>0 and deadcounter==-1) { shipx=--shipx; }      
  if (gb.buttons.repeat(BTN_RIGHT,1) and shipx<78 and deadcounter==-1) { shipx=++shipx; }
  if (gb.buttons.pressed(BTN_C)) {
    gb.titleScreen(F("    Yoda's"),gamelogo);
    gamestatus="title";
  }
  if (gb.buttons.pressed(BTN_A) and shotx==-1 and deadcounter==-1) {
    shotx=shipx+3;
    shoty=41;
    playsoundfx(0,0);
  }
}
//----------------------------------------------------------------------------
void drawplayership() {
  if (deadcounter == -1) {
    gb.display.drawBitmap(shipx,44,playership[0]);
  } else {
    gb.display.drawBitmap(shipx,44,playership[1+invadershotframe]);
    handledeath();
  }
}
//----------------------------------------------------------------------------
void drawplayershot() {
  if (shotx != -1) {
    shoty=shoty-2;
    gb.display.drawLine(shotx,shoty,shotx,shoty+2);
    if (shoty < 0) {
      shotx=-1;
      shoty=-1; 
    }
  }
}
//----------------------------------------------------------------------------
void invaderlogic() {
  // increment invader counter
  if (invaderanz>0){
    checkdir=0;
    do {
      invaderctr = ++invaderctr;
      if (invaderctr>39) {
         invaderctr=0;
         checkdir=1;
         invadersound=++invadersound % 4;
         playsoundfx(invadersound+4,1);
      }
    } while (invaders[invaderctr]==-1);

    // change direction?
    if (checkdir==1) {
      if (nextydir!=0) {
        invaderxr=0;
        invaderyr=2;
      }
      else {  
        invaderxr=nextxdir;
        invaderyr=0;
      }
      checkdir=0;
    }

    // change invader position
    invaderx[invaderctr]=invaderx[invaderctr]+invaderxr;
    invadery[invaderctr]=invadery[invaderctr]+invaderyr;

    // determine bunker removal if invaders are too low
    if (invadery[invaderctr] > 34) {
       for (int i = 0; i < 4; i++) {
         bunkers[i]=-1;
       } 
    }

    // determine game over if invaders reach bottom
    if (invadery[invaderctr] > 40) {
      gamestatus="gameover";
    }        

    // determine screen border hit -> go down, then change direction
    if (invaderx[invaderctr]>75 and invaderxr>0) {
      nextxdir=-2;
      nextydir=2;
    }
    if (invaderx[invaderctr]<2 and invaderxr<0) {
      nextxdir=2;
      nextydir=2;          
    }
    if (invaderyr!=0) { nextydir=0; }

    //change invader shape
    invaderframe[invaderctr]=++invaderframe[invaderctr] % 2;

    // remove killed invader
    if (invaders[invaderctr]==6) {
      invaders[invaderctr]=-1;
      invaderanz=--invaderanz;
    }

    // release invadershoot
    if (invadershottimer<=0 and invadershots<gamelevel+1 and invadershots<4 and invadery[invaderctr]<40) {
      invadershottimer=40-gamelevel*10;
      invadershots=++invadershots;  
      int flag=0;
      for (int u = 0 ; u < 4 ; u++) {
        if (flag == 0 and invadershotx[u]==-1) {
          invadershotx[u]=invaderx[invaderctr]+1;
          invadershoty[u]=invadery[invaderctr];
          flag=1;  
        }              
      }
    }
  }
}
//----------------------------------------------------------------------------
void drawinvaders() {
  infoshow=1;
  for (int i = 0; i < 40 ; i++) {
    if (invaders[i]!=-1) {
      gb.display.drawBitmap(invaderx[i],invadery[i],invader[invaders[i]+invaderframe[i]]);
      if (invadery[i]<5) {
        infoshow=0;
      }    
    }

    // determine collission: invader & player shoot
    checkl = invaderx[i];
    checkr = invaderx[i]+6;
    checkt = invadery[i];
    checkb = invadery[i]+4;
    if (invaders[i]==4) {
      checkl = ++checkl;
      checkr = --checkr;
    }
    if (invaders[i]!=-1 and invaders[i]!=6 and shotx>=checkl and shotx<=checkr and shoty+2>=checkt and shoty<=checkb) {
      score = score + invaders[i]*10+10;
      invaders[i]=6;
      shotx=-1;
      shoty=-1;
      playsoundfx(1,0);
    }  
  }
}
//----------------------------------------------------------------------------
void invadershot() {
  // handle invadershoot timer & framecounter
  invadershottimer = --invadershottimer;
  invadershotframe = ++invadershotframe % 2;

  // move invadershots, draw & check collission
  for (int i = 0 ; i < 4 ; i++) {
    if (invadershotx[i] != -1) {
      invadershoty[i] = invadershoty[i]+1;
      gb.display.drawBitmap(invadershotx[i],invadershoty[i],bomb[invadershotframe]);

  // check collission: invadershot & bunker
      for (int u = 0 ; u < 4 ; u++) {
        checkl = 11+u*18;
        checkr = 11+u*18+7;
        checkt = 39;
        checkb = 43;
        if (bunkers[u] !=-1 and invadershotx[i]+1>=checkl and invadershotx[i]<=checkr and invadershoty[i]+3>=checkt and invadershoty[i]<=checkb) {
          bunkers[u]=++bunkers[u];
          if (bunkers[u]>4) { bunkers[u]=-1; }
          invadershotx[i]=-1;
          invadershoty[i]=-1;
          invadershots=--invadershots; 
        }
      }
 
  // check collission: invadershot & player
        checkl = shipx;
        checkr = shipx+6;
        checkt = 44;
        checkb = 47;
        if (deadcounter==-1 and invadershotx[i]+1>=checkl and invadershotx[i]<=checkr and invadershoty[i]+3>=checkt and invadershoty[i]<=checkb) {
          deadcounter=60;            
          playsoundfx(2,2);
        }

  //check collission: invadershot & playershoot
        checkl = invadershotx[i];
        checkr = invadershotx[i]+1;
        checkt = invadershoty[i];
        checkb = invadershoty[i]+3;
        if (shotx>=checkl and shotx<=checkr and shoty+2>=checkt and shoty<=checkb) {
          shotx=-1;
          shoty=-1;  
          invadershotx[i]=-1;
          invadershoty[i]=-1;
          invadershots=--invadershots;           
        }

  // invadershoot on bottom off screen?
      if (invadershoty[i] > 47) {
        invadershotx[i]=-1;
        invadershoty[i]=-1;
        invadershots=--invadershots; 
      }
    }          
  }      
}
//----------------------------------------------------------------------------
void drawbunkers() {
  for (int i = 0; i < 4 ; i++) {
    checkl = 11+i*18;
    checkr = 11+i*18+7;
    checkt = 39;
    checkb = 43;
    if (bunkers[i] !=-1 and shotx>=checkl and shotx<=checkr and shoty+2>=checkt and shoty<=checkb) {
      bunkers[i]=++bunkers[i];
      shotx=-1;
      shoty=-1;
     if (bunkers[i]>4) { bunkers[i]=-1; }
    }

    if (bunkers[i] != -1) {
      gb.display.drawBitmap(11+i*18,39,bunker[bunkers[i]]);
    } 
  }
}
//----------------------------------------------------------------------------
void saucerappears() {
  saucertimer=--saucertimer;
  if (saucertimer<=0) {
    saucertimer=240;  
    if (infoshow==1 and saucers==-1) {
      saucers=0;
      int i = random(2);
      if (i==0) {
        saucerx=0;
        saucerdir=1;  
      } else {
        saucerx=73;
        saucerdir=-1;  
      }          
    }
  }
}
//----------------------------------------------------------------------------
void movesaucer() {
  if (saucers==0) {
    saucerx=saucerx+saucerdir;
    if (saucerx<=0 or saucerx>=73) {
      saucers=-1;
    }                
    if (saucerx % 5 == 0) { playsoundfx(3,0); }

    // check collission: player shot & saucer  
    checkl = saucerx;
    checkr = saucerx+10;
    checkt = 0;
    checkb = 3;
    if (shotx>=checkl and shotx<=checkr and shoty+2>=checkt and shoty<=checkb) {
      score=score+100;
      saucers=1;
      shotx=-1;
      shoty=-1;
      saucerwait=30;
      playsoundfx(1,0);
    }
  }
}
//----------------------------------------------------------------------------
void drawsaucer() {
  if (saucers != -1) {
    gb.display.drawBitmap(saucerx,0,saucer[saucers]);
    if (saucers==1) {
      saucerwait=--saucerwait;
      if (saucerwait<=0) {
        saucers=-1;
      }
    }
  }
}
//----------------------------------------------------------------------------

