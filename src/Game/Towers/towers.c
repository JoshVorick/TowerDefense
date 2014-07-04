#include "towers.h"

Enemy* findEnemyInRange(int x, int y, int radius, Enemy *curEnemy);
void towerShoot(Enemy *enemy, Tower *tower);
extern void drawImage(SDL_Surface *surface, int x, int y);
extern void addBullet(SubGame *subGame, Tower *tower, Enemy *enemy);

void updateTowers(SubGame *subGame){
  printf("update tower start\n");
  //Loop through each tower
  Tower *curTower = subGame->towers;
  while(curTower != NULL){
    curTower->ticksSinceFired++;
    //shoot things 
    if(curTower->ticksSinceFired >= curTower->fireRate){
      Enemy *gonnaGetShot = findEnemyInRange(curTower->x, curTower->y, curTower->range, subGame->enemies);
      if(gonnaGetShot != NULL){
        towerShoot(gonnaGetShot, curTower);
        addBullet(subGame, curTower, gonnaGetShot);
        curTower->ticksSinceFired = 0;
      }
    }
    curTower = curTower->next;
  }
  printf("update tower end\n");
};

int addTower(SubGame *subGame, int towerType, int towerPrice, SDL_Color rgbRatio){
  printf("add tower start\n");
  if(subGame->grid->blocksPath == TRUE)
    return FALSE;
  double total = (rgbRatio.r + rgbRatio.g + rgbRatio.b) / ((double)towerPrice);
  if(subGame->rStored < rgbRatio.r / total || subGame->gStored < rgbRatio.g/total || subGame->bStored < rgbRatio.b/total)
    return FALSE;
  
  subGame->rStored -= (int)(rgbRatio.r / total);
  subGame->gStored -= (int)(rgbRatio.g / total);
  subGame->bStored -= (int)(rgbRatio.b / total);

  Tower *newTower = malloc(sizeof(Tower));
  
  newTower->x = subGame->grid->selectedTile->x;
  newTower->y = subGame->grid->selectedTile->y;
  newTower->health = 100;
  newTower->color.r = rgbRatio.r;
  newTower->color.g = rgbRatio.g;
  newTower->color.b = rgbRatio.b;
  newTower->type = towerType;
  newTower->kills = 0;
  newTower->ticksSinceFired = 0;

  switch(towerType){
    case TRIANGLE:
      newTower->dmg = TRIANGLE_DMG;
      newTower->fireRate = TRIANGLE_FIRERATE;
      newTower->range = TRIANGLE_RANGE;
      break;
    case SQUARE:
      newTower->dmg = SQUARE_DMG;
      newTower->fireRate = SQUARE_FIRERATE;
      newTower->range = SQUARE_RANGE;
      break;
    case PENTAGON:
      newTower->dmg = PENTAGON_DMG;
      newTower->fireRate = PENTAGON_FIRERATE;
      newTower->range = PENTAGON_RANGE;
     break;
    case HEXAGON:
      newTower->dmg = HEXAGON_DMG;
      newTower->fireRate = HEXAGON_FIRERATE;
      newTower->range = HEXAGON_RANGE;
      break;
  }
    
  subGame->grid->selectedTile->myTower = newTower;
    
  //add to list of tower
  newTower->next = subGame->towers;
  subGame->towers = newTower;

  printf("add tower end\n");
  return TRUE;
};

Enemy* findEnemyInRange(int x, int y, int radius, Enemy *curEnemy){
  printf("find enemy start\n");
  while(curEnemy != NULL){
    if(curEnemy->health > 0 && (curEnemy->x - x)*(curEnemy->x - x) + (curEnemy->y - y)*(curEnemy->y - y) <= radius*radius)
      return curEnemy;
    else
      curEnemy = curEnemy-> next;
  }
  printf("find enemy end\n");
  return NULL;
};

void towerShoot(Enemy *enemy, Tower *tower){
  printf("tower shoot start\n");
  enemy->health -= tower->dmg;
  printf("tower shoot end\n");
};

void drawTowers(SubGame *subGame, Sprite sprites[NUM_SPRITES_GAME]){
  printf("draw towers start\n");
  Tower *curTower = subGame->towers;
  while(curTower != NULL){
    SDL_Rect rect = {curTower->x, curTower->y, 50, 50};
    SDL_FillRect(screen, &rect, SDL_MapRGB(screen->format, curTower->color.r, curTower->color.g, curTower->color.b));
    drawImage(sprites[curTower->type].image, curTower->x, curTower->y);
    curTower = curTower->next;
  }
  printf("draw towers end\n");
};
