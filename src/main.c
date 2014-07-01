/*
Some silly Tower Defense game I decided to start
Right now it sucks, I'll be honest.
Really not worth playing, guess its kinda fun maybe?
*/

#include "main.h"

//some useful funktions
extern void init(char*);
extern void cleanup(void);
extern void getInputStartMenu(StartMenu *startMenu);
extern void initStartMenu(StartMenu *startMenu);
extern void updateStartMenu(StartMenu *startMenu);
extern void drawStartMenu(StartMenu *startMenu);
extern void freeStartMenu(StartMenu *startMenu);
extern void initGame(Game *game);
extern void getInputGame(Game *game);
extern void updateGame(Game *game);
extern void drawGame(Game *game);
extern void freeGame(Game *game); //A free game!? Where?! Oh! Its right here!!

int main(void){
  unsigned int fpsLimit = 16;
  int playing = TRUE;
  
  init("HOLYMOLYTOWERSDEFENDINGTHINGS!!");

  atexit(cleanup);
  
  while(playing){
    //create and init start menu variables
    StartMenu *startMenu = malloc(sizeof(StartMenu)); 
    initStartMenu(startMenu);
    
    while(startMenu->atStartMenu){
      getInputStartMenu(startMenu);
      updateStartMenu(startMenu);
      drawStartMenu(startMenu);

      unsigned int ticks = SDL_GetTicks();
      if(ticks < fpsLimit)
         SDL_Delay(fpsLimit - ticks);
      fpsLimit = SDL_GetTicks() + 16;
    }
    
    freeStartMenu(startMenu);   
    
    //create and init game variables
    Game *game = malloc(sizeof(Game));
    initGame(game);
   
    while(game->inGame){
      getInputGame(game);
      updateGame(game);
      drawGame(game);

      unsigned int ticks = SDL_GetTicks();
      //printf("ticks since last frame: %i \n", ticks - fpsLimit);
      if(ticks < fpsLimit)
        SDL_Delay(fpsLimit - ticks);
      fpsLimit = SDL_GetTicks() + 16;
      //printf("ticks since last frame: %i \n", ticks - fpsLimit);
    }

    freeGame(game);
  }
  exit(0);
}
