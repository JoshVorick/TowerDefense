#include "towers.h"

Enemy* findEnemyInRange(int x, int y, int radius, Enemy *curEnemy);
void towerShoot(Enemy *enemy, Tower *tower);
extern void drawImage(SDL_Surface *surface, int x, int y);
extern void addBullet(SubGame *subGame, Tower *tower, Enemy *enemy);

void updateTowers(SubGame *subGame){
  //Loop through each tower
  Tower *curTower = subGame->towers;
  while(curTower != NULL){
    curTower->ticksSinceFired++;
    //shoot things 
    if(curTower->ticksSinceFired >= curTower->fireRate){
      Enemy *gonnaGetShot = findEnemyInRange(curTower->x, curTower->y, curTower->range, subGame->enemies);
      if(gonnaGetShot != NULL){
        towerShoot(gonnaGetShot, curTower);
        printf("HA\n");
        addBullet(subGame, curTower, gonnaGetShot);
        curTower->ticksSinceFired = 0;
      }
    }
    curTower = curTower->next;
  }
};

int addTower(Game *game, int towerType){
  if(game->subGames->grid->blocksPath == TRUE)
    return FALSE;
  double total = (game->rgbRatio.r + game->rgbRatio.g + game->rgbRatio.b) / (double)game->towerPrices[game->selectedTower];
  if(game->subGames->rStored < game->rgbRatio.r / total || game->subGames->gStored < game->rgbRatio.g/total || game->subGames->bStored < game->rgbRatio.b/total)
    return FALSE;
  
  game->subGames->rStored -= (int)(game->rgbRatio.r / total);
  game->subGames->gStored -= (int)(game->rgbRatio.g / total);
  game->subGames->bStored -= (int)(game->rgbRatio.b / total);

  Tower *newTower = malloc(sizeof(Tower));
  
  newTower->x = game->subGames->grid->selectedTile->x;
  newTower->y = game->subGames->grid->selectedTile->y;
  newTower->health = 100;
  newTower->color.r = game->rgbRatio.r;
  newTower->color.g = game->rgbRatio.g;
  newTower->color.b = game->rgbRatio.b;
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
    
  game->subGames->grid->selectedTile->myTower = newTower;
    
  //add to list of tower
  newTower->next = game->subGames->towers;
  game->subGames->towers = newTower;

  return TRUE;
};

Enemy* findEnemyInRange(int x, int y, int radius, Enemy *curEnemy){
  while(curEnemy != NULL){
    if(curEnemy->health > 0 && (curEnemy->x - x)*(curEnemy->x - x) + (curEnemy->y - y)*(curEnemy->y - y) <= radius*radius)
      return curEnemy;
    else
      curEnemy = curEnemy-> next;
  }
  return NULL;
};

void towerShoot(Enemy *enemy, Tower *tower){
  enemy->health -= tower->dmg;
};

void drawTowers(SubGame *subGame, Sprite sprites[NUM_SPRITES_GAME]){
  Tower *curTower = subGame->towers;
  while(curTower != NULL){
    SDL_Rect rect = {curTower->x, curTower->y, 50, 50};
    SDL_FillRect(screen, &rect, SDL_MapRGB(screen->format, curTower->color.r, curTower->color.g, curTower->color.b));
    drawImage(sprites[curTower->type].image, curTower->x, curTower->y);
    curTower = curTower->next;
  }
};
