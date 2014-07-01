#include "towers.h"

void addBullet(Game *game, Tower *tower, Enemy *enemy){
  Bullet *newBullet;
  newBullet = malloc(sizeof(Bullet));
  newBullet->origin = tower;
  newBullet->targetX = enemy->x;
  newBullet->targetY = enemy->y;
  newBullet->width = 5;//tower->dmg;
  newBullet->time = 0;
 
  newBullet->nextBullet = game->bullets;
  game->bullets = newBullet;
};

void updateBullets(Game *game){
  Bullet *curBullet = game->bullets;
  Bullet *prevBullet = NULL;
  while(curBullet != NULL){
    curBullet->time += 1;
    if(curBullet->time > 10){
      if(prevBullet != NULL)
        prevBullet->nextBullet = curBullet->nextBullet;
      else
        game->bullets = NULL;
      Bullet *temp = curBullet;
      curBullet = prevBullet;
      free(temp);
    }
    if(curBullet != NULL){
      prevBullet = curBullet;
      curBullet = curBullet->nextBullet;
    }
  }
};

void drawBullets(Game *game){
  Bullet *curBullet = game->bullets;
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
};
