#include "towers.h"

Enemy* findEnemyInRange(int x, int y, int radius, Enemy *curEnemy);
void towerShoot(Enemy *enemy, Tower *tower);
extern void drawImage(SDL_Surface *surface, int x, int y);
extern void addBullet(Game *game, Tower *tower, Enemy *enemy);

void updateTowers(Game *game){
  //Loop through each tower
  Tower *curTower = game->towers;
  while(curTower != NULL){
    curTower->ticksSinceFired++;
    //shoot things 
    if(curTower->ticksSinceFired >= curTower->fireRate){
      Enemy *gonnaGetShot = findEnemyInRange(curTower->x, curTower->y, curTower->range, game->enemies);
      if(gonnaGetShot != NULL){
        towerShoot(gonnaGetShot, curTower);
        printf("HA\n");
        addBullet(game, curTower, gonnaGetShot);
        curTower->ticksSinceFired = 0;
      }
    }
    curTower = curTower->nextTower;
  }
};

int addTower(Game *game, int towerType){
  if(game->grid->blocksPath == TRUE)
    return FALSE;
  double total = (game->rRatio + game->gRatio + game->bRatio) / (double)game->towerPrices[game->selectedTowerType];
  if(game->rStored < game->rRatio / total || game->gStored < game->gRatio/total || game->bStored < game->bRatio/total)
    return FALSE;
  
  game->rStored -= (int)(game->rRatio / total);
  game->gStored -= (int)(game->gRatio / total);
  game->bStored -= (int)(game->bRatio / total);

  Tower *newTower = malloc(sizeof(Tower));
  
  newTower->x = game->grid->selectedTile->x;
  newTower->y = game->grid->selectedTile->y;
  newTower->health = 100;
  newTower->color.r = game->rRatio;
  newTower->color.g = game->gRatio;
  newTower->color.b = game->bRatio;
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
    
  game->grid->selectedTile->myTower = newTower;
    
  //add to list of tower
  newTower->nextTower = game->towers;
  game->towers = newTower;

  return TRUE;
};

Enemy* findEnemyInRange(int x, int y, int radius, Enemy *curEnemy){
  while(curEnemy != NULL){
    if(curEnemy->health > 0 && (curEnemy->x - x)*(curEnemy->x - x) + (curEnemy->y - y)*(curEnemy->y - y) <= radius*radius)
      return curEnemy;
    else
      curEnemy = curEnemy-> nextEnemy;
  }
  return NULL;
};

void towerShoot(Enemy *enemy, Tower *tower){
  enemy->health -= tower->dmg;
};

void drawTowers(Game *game){
  Tower *curTower = game->towers;
  while(curTower != NULL){
    SDL_Rect rect = {curTower->x, curTower->y, 50, 50};
    SDL_FillRect(screen, &rect, SDL_MapRGB(screen->format, curTower->color.r, curTower->color.g, curTower->color.b));
    drawImage(game->sprites[curTower->type].image, curTower->x, curTower->y);
    curTower = curTower->nextTower;
  }
};
