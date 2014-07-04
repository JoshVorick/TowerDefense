#include "subGame.h"

extern void initGrid(Grid *grid, int x, int y);
extern void initEnemyGenerator(SubGame *subGame);
extern void generateEnemies(SubGame *subGame);
extern void updateTowers(SubGame *subGame);
extern void updateBullets(SubGame *subGame);
extern void updateEnemies(SubGame *subGame);
extern void findPath(SubGame *subGame);
extern void drawImage(SDL_Surface *surface, int x, int y);
extern void drawTowers(SubGame *subGame);
extern void drawBullets(SubGame *subGame);
extern void freeGrid(Grid *grid);

void updateSubGames(SubGame *subGame, int levelTime);
void freeSubGames(SubGame *subGame);

void initSubGame(SubGame *subGame) {
  subGame->lives = 20;
  subGame->rStored = 1500;
  subGame->gStored = 1500;
  subGame->bStored = 1500;
  
  initEnemyGenerator(subGame);
  subGame->towers = NULL;
  subGame->enemies = NULL;
  subGame->bullets = NULL;
  
  subGame->grid = malloc(sizeof(Grid));
  subGame->grid->dimensionX = 7;
  subGame->grid->dimensionY = 12;
  subGame->grid->tiles = (Grid_Tile**)malloc(subGame->grid->dimensionX*sizeof(Grid_Tile));
  initGrid(subGame->grid, 100, 60);
}

void updateSubGames(SubGame *subGame, int levelTime) {
  if(levelTime) {
    generateEnemies(subGame);
  }
  
  updateTowers(subGame);
  updateEnemies(subGame);
  updateBullets(subGame);
  if (subGame->next != NULL)
    updateSubGames(subGame->next, levelTime);
};

void drawSubGames(SubGame *subGame, Sprite sprites[NUM_SPRITES_GAME]) {
  int i,j;
  for(i=0; i<subGame->grid->dimensionX; i++) {
    for(j=0; j<subGame->grid->dimensionY; j++) {
      drawImage(sprites[GRID_TILE].image, subGame->grid->tiles[i][j].x, subGame->grid->tiles[i][j].y);
    }
  }
  drawTowers(subGame);
  
  Enemy *curEnemy = subGame->enemies;
  while(curEnemy != NULL) {
    drawImage(sprites[curEnemy->type].image, curEnemy->x, curEnemy->y);
    
    //draw hp bar
    SDL_Rect rect = {curEnemy->x, curEnemy->y, sprites[curEnemy->type].image->w, 4};
    SDL_FillRect(screen, &rect, 0xAAAAAA);
    rect.w = sprites[curEnemy->type].image->w * curEnemy->health / curEnemy->maxHealth;
    SDL_FillRect(screen, &rect, 0xFF0000);
  }
  
  drawBullets(subGame);  
};

void freeSubGames(SubGame *subGame) {
  freeGrid(subGame->grid);
  
  Enemy *temp, *curEnemy = subGame->enemies;
  while(curEnemy != NULL) {
    temp = curEnemy;
    curEnemy = curEnemy->next;
    free(temp);
  }
  Tower *temp1, *curTower = subGame->towers;
  while(curTower != NULL) {
    temp1 = curTower;
    curTower = curTower->next;
    free(temp1);
  }

  if(subGame->next != NULL) {
    freeSubGames(subGame->next);
  }
  free(subGame);
};
