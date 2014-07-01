#include "enemies.h"

void initEnemy(Enemy *enemy, int type, Grid_Tile *startGrid, Sprite sprites[]){
  enemy->x = startGrid->x - sprites[type].image->w/2 + sprites[GRID_TILE].image->w/2;
  enemy->y = startGrid->y - sprites[type].image->h/2;
  enemy->type = type;
  enemy->dir = DOWN;
  enemy->curTile = startGrid;
  enemy->nextEnemy = NULL;

  switch(type){
    case ENEMY1:
      enemy->speed = ENEMY1_SPEED;
      enemy->health = ENEMY1_HEALTH;
      enemy->maxHealth = ENEMY1_HEALTH;
      enemy->score = ENEMY1_SCORE;
      enemy->color.r = ENEMY1_R;
      enemy->color.g = ENEMY1_G;
      enemy->color.b = ENEMY1_B;
      break;
    case ENEMY2:
      enemy->speed = ENEMY2_SPEED;
      enemy->health = ENEMY2_HEALTH;
      enemy->maxHealth = ENEMY2_HEALTH;
      enemy->score = ENEMY2_SCORE;
      enemy->color.r = ENEMY2_R;
      enemy->color.g = ENEMY2_G;
      enemy->color.b = ENEMY2_B;
      break;
    default:
      break;
  }
}

Enemy* msortEnemies(Enemy *head){
  if(head == NULL)
    return NULL;

  Enemy *p, *q, *e, *tail;
  int insize, nmerges, psize, qsize, i;

  insize = 1;
  
  while(TRUE){
    p = head;
    head = NULL;
    tail = NULL;

    nmerges = 0;//num merges this pass

    while(p){
      nmerges++;//There is a merge that needs doing
      
      q = p;
      psize = 0;
      for(i=0; i < insize; i++){
        psize++;
        q = q->nextEnemy;
        if(q== NULL) 
          break;
      }

      qsize = insize;

      while(psize > 0 || (qsize > 0 && q)){
        //Decide if next enemy is in p or q
        if(psize == 0){
          e = q;
          q = q->nextEnemy;
          qsize--;
        }else if(qsize == 0 || q == NULL){
          e = p;
          p = p->nextEnemy;
          psize--;
        }else if(p->curTile != NULL && q->curTile != NULL && (p->curTile->distFromExit < q->curTile->distFromExit || (p->curTile->distFromExit == q->curTile->distFromExit && p->y > q->y))){
          e = p;
          p = p->nextEnemy;
          psize--;
        }else{
          e = q;
          q = q->nextEnemy;
          qsize--;
        }

        if(tail)
          tail->nextEnemy = e;
        else
          head = e;

        tail = e;
      }
      p = q;
    }
    tail->nextEnemy = NULL;
    
    if(nmerges <= 1)
      return head;

    insize *= 2;
  }
};

void updateEnemies(Game *game){
  Enemy *curEnemy = game->enemies;
  Enemy *prevEnemy = NULL;
  while(curEnemy != NULL){
    if (curEnemy->curTile == game->grid->endTile && curEnemy->y + game->sprites[curEnemy->type].image->h / 2 >= curEnemy->curTile->y) {
      curEnemy-> health = -9999;
      game->lives -= 1;
    }
    //Remove enemy if its dead, add to score
    if(curEnemy->health <= 0){
      if (curEnemy ->health > -9999) {
        game->score += curEnemy->score;
        game->rStored += curEnemy->color.r;
        game->gStored += curEnemy->color.g;
        game->bStored += curEnemy->color.b;
      }
      Enemy *temp = curEnemy;
      if(prevEnemy != NULL){//If curEnemy is not first in list
        prevEnemy->nextEnemy = curEnemy->nextEnemy;
        curEnemy = curEnemy->nextEnemy; //iterate to next enemy
      }else{//If curEnemy IS first in list
        game->enemies = curEnemy->nextEnemy;
        curEnemy = curEnemy->nextEnemy;//iterate to next enemy
      }
      free(temp);
    }else{
      //Move the enemy
      switch(curEnemy->dir){
      case LEFT:
        curEnemy->x -= curEnemy->speed;
        //If its reached middle of grid, start moving towards next grid in path 
        if(curEnemy->x + game->sprites[curEnemy->type].image->w/2 <= curEnemy->curTile->x + game->sprites[GRID_TILE].image->w/2){
          curEnemy->dir = curEnemy->curTile->dirToNextInPath;
          curEnemy->curTile = curEnemy->curTile->nextInPath;
        }
        break;
      case RIGHT:
        curEnemy->x += curEnemy->speed;
        //If its reached middle of grid, start moving towards next grid in path 
        if(curEnemy->x + game->sprites[curEnemy->type].image->w/2 >= curEnemy->curTile->x + game->sprites[GRID_TILE].image->w/2){
          curEnemy->dir = curEnemy->curTile->dirToNextInPath;
          curEnemy->curTile = curEnemy->curTile->nextInPath;
        }
        break;
      case UP:
        curEnemy->y -= curEnemy->speed;
        //If its reached middle of grid, start moving towards next grid in path 
        if(curEnemy->y + game->sprites[curEnemy->type].image->h/2 <= curEnemy->curTile->y + game->sprites[GRID_TILE].image->w/2){
          curEnemy->dir = curEnemy->curTile->dirToNextInPath;
          curEnemy->curTile = curEnemy->curTile->nextInPath;
        }
        break;
      case DOWN:
        curEnemy->y += curEnemy->speed;
        //If its reached middle of grid, start moving towards next grid in path 
        if(curEnemy->y + game->sprites[curEnemy->type].image->h/2 >= curEnemy->curTile->y + game->sprites[GRID_TILE].image->w/2){
          curEnemy->dir = curEnemy->curTile->dirToNextInPath;
          curEnemy->curTile = curEnemy->curTile->nextInPath;
        }
        break;
      }

      //iterate to next enemy
      prevEnemy = curEnemy;
      curEnemy = curEnemy->nextEnemy;
    }
  }
  game->enemies = msortEnemies(game->enemies);
};

