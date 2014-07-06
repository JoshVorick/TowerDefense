#include "game.h"

extern SDL_Surface *loadImage(char *name);
extern void initInputGame(Game *game);
extern void initSubGame(SubGame *subGame);
extern int updateSubGames(SubGame *subGame, Sprite sprites[NUM_SPRITES_GAME]);
extern void drawString(char *text, int x, int y, TTF_Font *font, int centerX, int centerY, SDL_Color foregroundColor, SDL_Color backgroundColor);
extern void drawImage(SDL_Surface *surface, int x, int y);
extern void drawSubGames(SubGame *subGame, Sprite sprites[NUM_SPRITES_GAME]);
extern void closeFont(TTF_Font *);
extern void freeSubGames(SubGame *subGame);

void initGame(Game *game){
  printf("init game start\n");
  game->totalTime = 0;
  game->inGame = TRUE;
  game->score = 0;
  game->selectedTower = TRIANGLE;
  game->rgbRatio.r = 90;
  game->rgbRatio.g = 90;
  game->rgbRatio.b = 90;
  
  int i;
  for(i=0;i<NUM_GAME_KEYS;i++)
    game->keys[i] = FALSE;
  
  game->towerPrices[TRIANGLE] = TRIANGLE_PRICE;
  game->towerPrices[SQUARE] = SQUARE_PRICE;
  game->towerPrices[PENTAGON] = PENTAGON_PRICE;
  game->towerPrices[HEXAGON] = HEXAGON_PRICE;

  initInputGame(game);

  game->sprites[gBACKGROUND].image = loadImage("img/GameBackground.png");
  game->sprites[GRID_TILE].image = loadImage("img/Grid_Tile.png");
  game->sprites[ENEMY1].image = loadImage("img/Alien1.png");
  game->sprites[ENEMY2].image = loadImage("img/Alien2.png");
  game->sprites[TRIANGLE].image = loadImage("img/TriangleTower.png");
  game->sprites[SQUARE].image = loadImage("img/SquareTower.png");
  game->sprites[PENTAGON].image = loadImage("img/PentagonTower.png");
  game->sprites[HEXAGON].image = loadImage("img/HexagonTower.png");  

  game->font = TTF_OpenFont("fonts/blackWolf.ttf", 14);
  game->white.r = 200;
  game->white.g = 200;
  game->white.b = 200;
  game->red.r = 230;
  game->red.g = 0;
  game->red.b = 0;
  game->green.r = 0;
  game->green.g = 230;
  game->green.b = 0;
  game->blue.r = 50;
  game->blue.g = 50;
  game->blue.b = 255;
  game->gray.r = 10;
  game->gray.g = 10;
  game->gray.b = 10;

  game->subGames = malloc(sizeof(SubGame));
  initSubGame(game->subGames);
  printf("init game end\n");
};

void updateGame(Game *game){
  printf("update game start\n");
  game->totalTime++;
  game->score += updateSubGames(game->subGames, game->sprites);
  printf("update game end\n");
};

void drawGame(Game *game){
  printf("draw game start\n");
  drawImage(game->sprites[gBACKGROUND].image, 0, 0);
 
  drawSubGames(game->subGames, game->sprites);
  
  SDL_Rect rect = {game->subGames->grid->selectedTile->x+20, game->subGames->grid->selectedTile->y+20, 10, 10};
  SDL_FillRect(screen, &rect, SDL_MapRGB(screen->format, game->rgbRatio.r, game->rgbRatio.g, game->rgbRatio.b));
 
  char str[20];
  SubGame *curSub = game->subGames;
  while(curSub != NULL) {
    drawString("Press M to go to Menu", 0, 667, game->font, 1, 0, game->white, game->gray);
    sprintf(str, "Score: %d", game->score);
    drawString(str, 0, 650, game->font, 1, 0, game->white, game->gray);
    sprintf(str, "Red: %i", game->subGames->rStored);
    drawString(str, 0, 17, game->font, 0, 0, game->red, game->gray);
    sprintf(str, "Green: %i", game->subGames->gStored);
    drawString(str, 0, 34, game->font, 0, 0, game->green, game->gray);
    sprintf(str, "Blue: %i", game->subGames->bStored);
    drawString(str, 0, 51, game->font, 0, 0, game->blue, game->gray);
    sprintf(str, "R:G:B =  %i, %i, %i", game->rgbRatio.r, game->rgbRatio.g, game->rgbRatio.b);
    drawString(str, 0, 0, game->font, 0, 0, game->white, game->gray);
    sprintf(str, "Lives: %i", game->subGames->lives);
    drawString(str, 0, 68, game->font, 0, 0, game->white, game->gray);
    
    curSub = curSub->next;
  }
  double total = (game->rgbRatio.r + game->rgbRatio.g + game->rgbRatio.b) / (double)game->towerPrices[game->selectedTower];
  sprintf(str, "Cost for selected Tower: %d,%d,%d", (int)(game->rgbRatio.r/total), (int)(game->rgbRatio.g/total), (int)(game->rgbRatio.b/total));
  drawString(str, 0, 0, game->font, 0, 0, game->white, game->gray);
  
  SDL_Flip(screen);
  printf("draw game end\n");
};

void freeGame(Game* game){
  printf("free game start\n");
  int i;
  for(i=0; i<NUM_SPRITES_GAME; i++)
    if(game->sprites[i].image != NULL)
      SDL_FreeSurface(game->sprites[i].image);
  
  TTF_CloseFont(game->font);

  freeSubGames(game->subGames);
  free(game);
  printf("free game end\n");
};
