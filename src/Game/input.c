#include "input.h"

extern int addTower(Game *game, int towerType);
extern void wouldBlockPath(Grid *grid);
extern void findPath(Grid *grid);
extern void freeGame(Game *game);

void initInputGame(Game *game){
};

void getInputGame(Game *game){
  SDL_Event event;

  while(SDL_PollEvent(&event)){
    switch(event.type){
      case SDL_QUIT:
        freeGame(game);
        exit(0);
        break;
      case SDL_KEYDOWN:
        switch(event.key.keysym.sym){
          case SDLK_ESCAPE:
            freeGame(game);
            exit(0);
            break;
          case SDLK_m:
            game->inGame = FALSE;
            break;
          
          case SDLK_1:
            game->selectedTowerType = TRIANGLE;
            game->keys[ONE] = TRUE;
            break;
          case SDLK_2:
            game->selectedTowerType = SQUARE;
            game->keys[TWO] = TRUE;
            break;
          case SDLK_3:
            game->selectedTowerType = PENTAGON;
            game->keys[THREE] = TRUE;
            break;
          case SDLK_4:
            game->selectedTowerType = HEXAGON;
            game->keys[FOUR] = TRUE;
            break;

          case SDLK_RSHIFT:
          case SDLK_LSHIFT:
            game->keys[SHIFT] = TRUE;
            break;

          case SDLK_q:
            game->rRatio += 30 - 29*game->keys[SHIFT];
            if(game->rRatio > 255)
              game->rRatio = 255;
            game->keys[Q] = TRUE;
            break;
          case SDLK_a:
            game->rRatio -= 30 - 29*game->keys[SHIFT];
            if(game->rRatio < 1)
              game->rRatio = 1;
            game->keys[A] = TRUE;
            break;
          case SDLK_w:
            game->gRatio += 30 - 29*game->keys[SHIFT];
            if(game->gRatio >255)
              game->gRatio = 255;
            game->keys[W] = TRUE;
            break;
          case SDLK_s:
            game->gRatio -= 30 - 29*game->keys[SHIFT];
            if(game->gRatio < 1)
              game->gRatio = 1;
            game->keys[S] = TRUE;
            break;
          case SDLK_e:
            game->bRatio += 30 - 29*game->keys[SHIFT];
            if(game->bRatio > 255)
              game->bRatio = 255;
            game->keys[E] = TRUE;
            break;
          case SDLK_d:
            game->bRatio -= 20 - 29*game->keys[SHIFT];
            if(game->bRatio < 1)
              game->bRatio = 1;
            game->keys[D] = TRUE;
            break;
          
          case SDLK_SPACE:
            if(addTower(game, game->selectedTowerType) == TRUE)//returns true if tower was added
              findPath(game->grid);
            game->keys[SPACE] = TRUE;
            break;
          
          case SDLK_UP:
          case SDLK_k:
            if(game->grid->selectedTile->j > 0){
              //move selectedTile
              game->grid->selectedTile = &game->grid->tiles[game->grid->selectedTile->i][game->grid->selectedTile->j-1];
              //recalculate if adding a tower here would block the path
              wouldBlockPath(game->grid);
            }
            game->keys[K] = TRUE;
            break;
          case SDLK_LEFT:
          case SDLK_h:
            if(game->grid->selectedTile->i > 0){
              //move selectedTile
              game->grid->selectedTile = &game->grid->tiles[game->grid->selectedTile->i-1][game->grid->selectedTile->j];
              //recalc if adding a tower here would block path
              wouldBlockPath(game->grid);
            }
            game->keys[H] = TRUE;
            break;
          case SDLK_DOWN:
          case SDLK_j:
            if(game->grid->selectedTile->j < game->grid->dimensionY-1){
              //move selectedTile
              game->grid->selectedTile = &game->grid->tiles[game->grid->selectedTile->i][game->grid->selectedTile->j + 1];
              //recalculate if adding a tower here would block path
              wouldBlockPath(game->grid);
            }
            game->keys[J] = TRUE;
            break;
          case SDLK_RIGHT:
          case SDLK_l:
            if(game->grid->selectedTile->i < game->grid->dimensionX-1){
              //move selectedTile
              game->grid->selectedTile = &game->grid->tiles[game->grid->selectedTile->i+1][game->grid->selectedTile->j];
              //recalculate if adding a tower here would block path
              wouldBlockPath(game->grid);
            }
            game->keys[L] = TRUE;
            break;
          
          case SDLK_RETURN:
            game->keys[RETURN] = TRUE;
            if(game->levelTime < 1 && game->enemies == NULL)
              game->levelTime = 1;
            break;

          default:
            break;
        }
        break;
      case SDL_KEYUP:
        switch(event.key.keysym.sym){
          case SDLK_1:
            game->keys[ONE] = FALSE;
            break;
          case SDLK_2:
            game->keys[TWO] = FALSE;
            break;
          case SDLK_3:
            game->keys[THREE] = FALSE;
            break;
          case SDLK_4:
            game->keys[FOUR] = FALSE;
            break;

          case SDLK_RSHIFT:
          case SDLK_LSHIFT:
            game->keys[SHIFT] = FALSE;
            break;

          case SDLK_q:
            game->keys[Q] = FALSE;
            break;
          case SDLK_a:
            game->keys[A] = FALSE;
            break;
          case SDLK_w:
            game->keys[W] = FALSE;
            break;
          case SDLK_s:
            game->keys[S] = FALSE;
            break;
          case SDLK_e:
            game->keys[E] = FALSE;
            break;
          case SDLK_d:
            game->keys[D] = FALSE;
            break;

          case SDLK_SPACE:
            game->keys[SPACE] = FALSE;
            break;

          case SDLK_UP:
          case SDLK_k:
            game->keys[K] = FALSE;
            break;
          case SDLK_LEFT:
          case SDLK_h:
            game->keys[H] = FALSE;
            break;
          case SDLK_DOWN:
          case SDLK_j:
            game->keys[J] = FALSE;
            break;
          case SDLK_RIGHT:
          case SDLK_l:
            game->keys[L] = FALSE;
            break;
          
          case SDLK_RETURN:
            game->keys[RETURN] = FALSE;
            break;
          default:
            break;
        }
        break;
    }
  }
};
