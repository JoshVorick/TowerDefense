#include "startMenu.h"

extern void initInputStartMenu(StartMenu *startMenu);
extern void drawString(char *text, int x, int y, TTF_Font *font, int centerX, int centerY, SDL_Color foregroundColor, SDL_Color backgroundColor);
extern SDL_Surface *loadImage(char* name);
extern void drawImage(SDL_Surface *surface, int x, int y);

void initStartMenu(StartMenu *startMenu){
  //assign variables
  startMenu->atStartMenu = TRUE;
  initInputStartMenu(startMenu);
  startMenu->font = TTF_OpenFont("fonts/blackWolf.ttf", 16);  
  startMenu->fontColor.r = 55;
  startMenu->fontColor.g = 255;
  startMenu->fontColor.b = 55;
  startMenu->fontBGColor.r = 100;
  startMenu->fontBGColor.g = 0;
  startMenu->fontBGColor.b = 0;

  //load images
  startMenu->sprites[sBACKGROUND].image = loadImage("img/StartBackground.png");
};

void updateStartMenu(StartMenu *startMenu){

};

void drawStartMenu(StartMenu *startMenu){
  drawImage(startMenu->sprites[sBACKGROUND].image, 0, 0);
  
  char *str = "Press any key to start game!";
  drawString(str, 0, 0, startMenu->font, 1, 1, startMenu->fontColor, startMenu->fontBGColor);

  SDL_Flip(screen);
};

void freeStartMenu(StartMenu *startMenu){
  //Set the variables free!!
  TTF_CloseFont(startMenu->font);
  int i;
  for(i=0;i<NUM_SPRITES_STARTMENU;i++)
    SDL_FreeSurface(startMenu->sprites[i].image);
};
