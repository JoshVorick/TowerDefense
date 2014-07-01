#include "grid.h"

void initGrid(Grid* grid, int x, int y){
  int i;
  for(i=0; i<grid->dimensionX; i++)
    grid->tiles[i] = (Grid_Tile*)malloc(grid->dimensionY * sizeof(Grid_Tile));

  int j;
  for(i=0; i<grid->dimensionX; i++)
    for(j=0; j<grid->dimensionY; j++){
      grid->tiles[i][j].x = x + i*50;
      grid->tiles[i][j].y = y + j*50;
      grid->tiles[i][j].i = i;
      grid->tiles[i][j].j = j;
      grid->tiles[i][j].distFromExit = 0;
      grid->tiles[i][j].myTower = NULL;
      if(j < grid->dimensionY - 1){ 
        grid->tiles[i][j].nextInPath = &grid->tiles[i][j+1];
        grid->tiles[i][j].dirToNextInPath = DOWN;
      }else{
        grid->tiles[i][j].nextInPath = NULL;
        grid->tiles[i][j].dirToNextInPath = DOWN;
      }
    }
  grid->selectedTile = &grid->tiles[0][0];
  grid->blocksPath = FALSE;
  grid->startTile = &grid->tiles[3][0];
  grid->endTile = malloc(sizeof(Grid_Tile));
  grid->endTile->distFromExit = 0;
  grid->endTile->dirToNextInPath = DOWN;
  grid->endTile->nextInPath = NULL;
  grid->endTile->next = NULL;
  grid->endTile->x = grid->tiles[3][grid->dimensionY-1].x + 50;
  grid->endTile->y = grid->tiles[3][grid->dimensionY-1].y + 50;
};

void freeGrid(Grid* grid){
  int i;
  for(i=0; i<grid->dimensionX; i++)
    free(grid->tiles[i]);
  free(grid);
};
