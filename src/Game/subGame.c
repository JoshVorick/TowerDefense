#include "subGame.h"

extern void initGrid(Grid *grid, int x, int y);
extern void initEnemyGenerator(EnemyGenerator *enemyGenerator);
extern void generateEnemies(SubGame *subGame, Sprite sprites[NUM_SPRITES_GAME]);
extern void updateTowers(SubGame *subGame);
extern void updateBullets(SubGame *subGame);
extern int updateEnemies(SubGame *subGame, Sprite sprites[NUM_SPRITES_GAME]);
extern void drawImage(SDL_Surface *surface, int x, int y);
extern void drawTowers(SubGame *subGame, Sprite sprites[NUM_SPRITES_GAME]);
extern void drawBullets(SubGame *subGame, Sprite sprites[NUM_SPRITES_GAME]);
extern void freeGrid(Grid *grid);

int updateSubGames(SubGame *subGame, Sprite sprites[NUM_SPRITES_GAME]);
void drawSubGames(SubGame *subGame, Sprite sprites[NUM_SPRITES_GAME]);
void freeSubGames(SubGame *subGame);

void initSubGame(SubGame *subGame) {
  printf("init subgame start\n");
  subGame->lives = 20;
  subGame->rStored = 1500;
  subGame->gStored = 1500;
  subGame->bStored = 1500;
  
  subGame->enemyGenerator = malloc(sizeof(EnemyGenerator));
  initEnemyGenerator(subGame->enemyGenerator);
  subGame->towers = NULL;
  subGame->enemies = NULL;
  subGame->bullets = NULL;
  
  subGame->grid = malloc(sizeof(Grid));
  subGame->grid->dimensionX = 7;
  subGame->grid->dimensionY = 12;
  subGame->grid->tiles = (Grid_Tile**)malloc(subGame->grid->dimensionX*sizeof(Grid_Tile));
  initGrid(subGame->grid, 100, 60);
  printf("init subgame end\n");
}

int updateSubGames(SubGame *subGame, Sprite sprites[NUM_SPRITES_GAME]) {
  printf("update subgame start\n");
  int score = 0;
  generateEnemies(subGame, sprites);
  
  updateTowers(subGame);
  score += updateEnemies(subGame, sprites);
  updateBullets(subGame);
  if (subGame->next != NULL)
    score += updateSubGames(subGame->next, sprites);
  printf("update subgame end\n");
  return score;
};

void drawSubGames(SubGame *subGame, Sprite sprites[NUM_SPRITES_GAME]) {
  printf("draw subgame start\n");
  int i,j;
  for(i=0; i<subGame->grid->dimensionX; i++) {
    for(j=0; j<subGame->grid->dimensionY; j++) {
      drawImage(sprites[GRID_TILE].image, subGame->grid->tiles[i][j].x, subGame->grid->tiles[i][j].y);
    }
  }
  drawTowers(subGame, sprites);
  
  Enemy *curEnemy = subGame->enemies;
  while(curEnemy != NULL) {
    drawImage(sprites[curEnemy->type].image, curEnemy->x, curEnemy->y);
    
    //draw hp bar
    SDL_Rect rect = {curEnemy->x, curEnemy->y, sprites[curEnemy->type].image->w, 4};
    SDL_FillRect(screen, &rect, 0xAAAAAA);
    rect.w = sprites[curEnemy->type].image->w * curEnemy->health / curEnemy->maxHealth;
    SDL_FillRect(screen, &rect, 0xFF0000);

    curEnemy = curEnemy->next;
  }
  
  drawBullets(subGame, sprites);  

  if (subGame->next != NULL)
    drawSubGames(subGame->next, sprites);
  printf("draw subgame end\n");
};

void freeSubGames(SubGame *subGame) {
  printf("free sub game start\n");
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
  printf("free sub game end\n");
};
