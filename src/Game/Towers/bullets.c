#include "towers.h"

void addBullet(SubGame *subGame, Tower *tower, Enemy *enemy){
  printf("add bullet start\n");
  Bullet *newBullet;
  newBullet = malloc(sizeof(Bullet));
  newBullet->origin = tower;
  newBullet->targetX = enemy->x;
  newBullet->targetY = enemy->y;
  newBullet->width = 5;//tower->dmg;
  newBullet->time = 0;
 
  newBullet->nextBullet = subGame->bullets;
  subGame->bullets = newBullet;
  printf("add bullet end\n");
};

void updateBullets(SubGame *subGame){
  printf("update bullet start\n");
  Bullet *curBullet = subGame->bullets;
  Bullet *prevBullet = NULL;
  while(curBullet != NULL){
    curBullet->time += 1;
    if(curBullet->time > 10){
      if(prevBullet != NULL)
        prevBullet->nextBullet = curBullet->nextBullet;
      else
        subGame->bullets = NULL;
      Bullet *temp = curBullet;
      curBullet = prevBullet;
      free(temp);
    }
    if(curBullet != NULL){
      prevBullet = curBullet;
      curBullet = curBullet->nextBullet;
    }
  }
  printf("update bullet end\n");
};

void drawBullets(SubGame *subGame){
  printf("draw bullet start\n");
  Bullet *curBullet = subGame->bullets;
  float x, y, stepX, stepY;
  int  i;
  while(curBullet != NULL){
    x = curBullet->origin->x + 20;
    y = curBullet->origin->y + 20;
    stepX = (curBullet->targetX - x + 10)/25;
    stepY = (curBullet->targetY - y + 10)/25;
    for(i=0; i<=25; i++){
      SDL_Rect rect = {x, y, curBullet->width,curBullet->width};
      SDL_FillRect(screen, &rect, SDL_MapRGB(screen->format, curBullet->origin->color.r, curBullet->origin->color.g, curBullet->origin->color.b));
      x += stepX;
      y += stepY;
    }
    curBullet = curBullet->nextBullet;
  }
  printf("draw bullet end\n");
};
