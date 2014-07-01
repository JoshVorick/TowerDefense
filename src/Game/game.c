#include "game.h"

extern void initInputGame(Game *game);
extern void initGrid(Grid* grid, int x, int y);
extern void initEnemyGenerator(Game*game);
extern void addEnemies(Game *game);
extern void updateTowers(Game *game);
extern void updateBullets(Game *game);
extern void updateEnemies(Game *game);
extern Enemy* msortEnemies(Enemy* enemy);
extern void closeFont(TTF_Font *);
extern void drawString(char *text, int x, int y, TTF_Font *font, int centerX, int centerY, SDL_Color foregroundColor, SDL_Color backgroundColor);
extern SDL_Surface *loadImage(char *name);
extern void drawImage(SDL_Surface *surface, int x, int y);
extern void drawTowers(Game *game);
extern void drawBullets(Game *game);
extern void freeGrid(Grid *grid);
extern void findPath(Game *game);

void initGame(Game *game){
  game->totalTime = 0;
  game->levelTime = 0;
  game->inGame = TRUE;
  game->score = 0;
  game->selectedTowerType = TRIANGLE;
  game->lives = 20;
  game->rStored = 3000;
  game->gStored = 3000;
  game->bStored = 3000;
  game->rRatio = 20;
  game->gRatio = 20;
  game->bRatio = 20;
  
  int i;
  for(i=0;i<NUM_GAME_KEYS;i++)
    game->keys[i] = FALSE;
  
  game->towerPrices[TRIANGLE] = TRIANGLE_PRICE;
  game->towerPrices[SQUARE] = SQUARE_PRICE;
  game->towerPrices[PENTAGON] = PENTAGON_PRICE;
  game->towerPrices[HEXAGON] = HEXAGON_PRICE;

  initInputGame(game);
  initEnemyGenerator(game);
  game->towers = NULL;
  game->enemies = NULL;
  game->bullets = NULL;

  game->sprites[gBACKGROUND].image = loadImage("img/GameBackground.png");
  game->sprites[GRID_TILE].image = loadImage("img/Grid_Tile.png");
  game->sprites[ENEMY1].image = loadImage("img/Alien1.png");
  game->sprites[ENEMY2].image = loadImage("img/Alien2.png");
  game->sprites[TRIANGLE].image = loadImage("img/TriangleTower.png");
  game->sprites[SQUARE].image = loadImage("img/SquareTower.png");
  game->sprites[PENTAGON].image = loadImage("img/PentagonTower.png");
  game->sprites[HEXAGON].image = loadImage("img/HexagonTower.png");  

  game->font = TTF_OpenFont("fonts/blackWolf.ttf", 16);
  game->fontColor.r = 200;
  game->fontColor.g = 200;
  game->fontColor.b = 200;
  game->fontBGColor.r = 30;
  game->fontBGColor.g = 30;
  game->fontBGColor.b = 30;

  game->grid = malloc(sizeof(Grid));
  game->grid->dimensionX = 7;
  game->grid->dimensionY = 12;
  game->grid->tiles = (Grid_Tile**)malloc(game->grid->dimensionX*sizeof(Grid_Tile));
  initGrid(game->grid, 75, 50);
};

void updateGame(Game *game){
  game->totalTime++;
  if(game->levelTime){
    game->levelTime++;
    addEnemies(game);//Adds enemies and checks if level is finished
  }

  updateTowers(game);
  updateEnemies(game);
  updateBullets(game);
};

void drawGame(Game *game){
  drawImage(game->sprites[gBACKGROUND].image, 0, 0);
  
  int i,j;
  
  for(i=0; i < game->grid->dimensionX; i++)
    for(j=0; j < game->grid->dimensionY; j++){
      drawImage(game->sprites[GRID_TILE].image, game->grid->tiles[i][j].x, game->grid->tiles[i][j].y);
      char str[1];
      sprintf(str, "%i", game->grid->tiles[i][j].distFromExit);
      drawString(str, game->grid->tiles[i][j].x, game->grid->tiles[i][j].y, game->font, 0, 0, game->fontColor, game->fontBGColor);
  }

  drawTowers(game);

  Enemy *curEnemy = game->enemies;
  while(curEnemy != NULL){
    drawImage(game->sprites[curEnemy->type].image, curEnemy->x, curEnemy->y);

    //draw health bar
    SDL_Rect rect = {curEnemy->x, curEnemy->y, game->sprites[curEnemy->type].image->w, 4};
    SDL_FillRect(screen, &rect, 0xAAAAAA);
    rect.w = game->sprites[curEnemy->type].image->w * curEnemy->health / curEnemy->maxHealth;
    SDL_FillRect(screen, &rect, 0xFF0000);
    curEnemy = curEnemy->nextEnemy;
  }
  
  drawBullets(game);
  
  SDL_Rect rect = {game->grid->selectedTile->x+20, game->grid->selectedTile->y+20, 10, 10};
  SDL_FillRect(screen, &rect, SDL_MapRGB(screen->format, game->rRatio, game->gRatio, game->bRatio));
  
  char str[20];
  sprintf(str, "Press M to go to menu. Score: %d", game->score);
  drawString(str, 0, 650, game->font, 1, 0, game->fontColor, game->fontBGColor);
  sprintf(str, "Red: %i", game->rStored);
  drawString(str, 0, 20, game->font, 0, 0, game->fontColor, game->fontBGColor);
  sprintf(str, "Green: %i", game->gStored);
  drawString(str, 0, 40, game->font, 0, 0, game->fontColor, game->fontBGColor);
  sprintf(str, "Blue: %i", game->bStored);
  drawString(str, 0, 60, game->font, 0, 0, game->fontColor, game->fontBGColor);
  sprintf(str, "R:G:B =  %i, %i, %i", game->rRatio, game->gRatio, game->bRatio);
  drawString(str, 0, 0, game->font, 0, 0, game->fontColor, game->fontBGColor);
  sprintf(str, "Lives left: %i", game->lives);
  drawString(str, 0, 80, game->font, 0, 0, game->fontColor, game->fontBGColor);
  
  double total = (game->rRatio + game->gRatio + game->bRatio) / (double)game->towerPrices[game->selectedTowerType];
  sprintf(str, "Cost for selected Tower: %d,%d,%d", (int)(game->rRatio/total), (int)(game->gRatio/total), (int)(game->bRatio/total));
  drawString(str, 0, 0, game->font, 0, 0, game->fontColor, game->fontBGColor);
  
  SDL_Flip(screen);
};

void freeGame(Game* game){
  int i;
  for(i=0; i<NUM_SPRITES_GAME; i++)
    if(game->sprites[i].image != NULL)
      SDL_FreeSurface(game->sprites[i].image);
  
  freeGrid(game->grid);
  TTF_CloseFont(game->font);

  Enemy *curEnemy = game->enemies;
  Enemy *tempE;
  while(curEnemy != NULL){
    tempE = curEnemy;
    curEnemy = curEnemy->nextEnemy;
    free(tempE);
  }

  Tower *curTower = game->towers;
  Tower *tempT;
  while(curTower != NULL){
    tempT = curTower;
    curTower = curTower->nextTower;
    free(tempT);
  }
};
