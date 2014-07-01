/*
How it works:
findPath-
  Rather than have each enemy store a path (some enemies may need different 
  paths than others) for them to follow, I realized it would be easier to simply
  have each grid point to the adjacent grid that is closest to the exit, then
  the enemies just follow these 'arrows'. Findpath finds these 'arrows' by
  starting at the tile next to the exit, and then iterating out to its adjacent
  tiles and having them point back to the tile before them until every
  unoccupied tile has an 'arrow'
  
  Big O notation is O(n) (iterates through each grid tile once)
  time to find optimal path to exit from every tile on a grid of 4,000,000 tiles: ~650 milliseconds
  A quicksort of 4,000,000 integers on the same computer took ~1060 milliseconds
*/
#include "pathFinding.h"

//calculate the path. Return TRUE if there is one, FALSE if there isn't one
int findPath(Grid *grid){
  int startTicks = SDL_GetTicks();//to find start time
  
  int i,j;
  for(i=0;i<grid->dimensionX; i++) {
    for(j=0;j<grid->dimensionY; j++) {
      grid->tiles[i][j].dirToNextInPath = -1;
      grid->tiles[i][j].distFromExit = -1;
      grid->tiles[i][j].nextInPath = NULL;
      grid->tiles[i][j].next = NULL;
    } 
  }
  Grid_Tile *pathless; //used to iterate through tiles
  if(grid->tiles[3][grid->dimensionY-1].myTower == NULL){
    grid->tiles[3][grid->dimensionY-1].distFromExit = 1;
    grid->tiles[3][grid->dimensionY-1].dirToNextInPath = DOWN;
    grid->tiles[3][grid->dimensionY-1].nextInPath = grid->endTile;
    grid->tiles[3][grid->dimensionY-1].next = NULL;

    pathless = &grid->tiles[3][grid->dimensionY-1]; //start at end tile
  }else
    return FALSE;

  while(pathless != NULL){
    Grid_Tile *curTile = pathless;
    Grid_Tile *prevTile = pathless; //used to remove tiles 
    while(curTile != NULL){
      
      //add path to left tile
      if(curTile->i > 0 && grid->tiles[curTile->i-1][curTile->j].nextInPath == NULL && grid->tiles[curTile->i-1][curTile->j].myTower == NULL){
        grid->tiles[curTile->i-1][curTile->j].nextInPath = curTile;
        grid->tiles[curTile->i-1][curTile->j].dirToNextInPath = RIGHT;
        grid->tiles[curTile->i-1][curTile->j].distFromExit = curTile->distFromExit + 1;
        
        //add to array so its adjacent blocks will be calculated
        grid->tiles[curTile->i-1][curTile->j].next = pathless;
        pathless = &grid->tiles[curTile->i-1][curTile->j];
      }
      
      //add path to right tile
      if(curTile->i < grid->dimensionX - 1 && grid->tiles[curTile->i+1][curTile->j].nextInPath == NULL && grid->tiles[curTile->i+1][curTile->j].myTower == NULL){
        grid->tiles[curTile->i+1][curTile->j].nextInPath = curTile;
        grid->tiles[curTile->i+1][curTile->j].dirToNextInPath = LEFT;
        grid->tiles[curTile->i+1][curTile->j].distFromExit = curTile->distFromExit + 1;

        //add to array so its adjacent blocks will be calculated
        grid->tiles[curTile->i+1][curTile->j].next = pathless;
        pathless = &grid->tiles[curTile->i+1][curTile->j];
      }
      
      //add path to top tile
      if(curTile->j > 0 && grid->tiles[curTile->i][curTile->j-1].nextInPath == NULL && grid->tiles[curTile->i][curTile->j-1].myTower == NULL){
        grid->tiles[curTile->i][curTile->j-1].nextInPath = curTile;
        grid->tiles[curTile->i][curTile->j-1].dirToNextInPath = DOWN;
        grid->tiles[curTile->i][curTile->j-1].distFromExit = curTile->distFromExit + 1;

        //add to array so its adjacent blocks will be calculated
        grid->tiles[curTile->i][curTile->j-1].next = pathless;
        pathless = &grid->tiles[curTile->i][curTile->j-1];
      }
      
      //add path to bottom tile
      if(curTile->j < grid->dimensionY-1 && grid->tiles[curTile->i][curTile->j+1].nextInPath == NULL && grid->tiles[curTile->i][curTile->j+1].myTower == NULL){
        grid->tiles[curTile->i][curTile->j+1].nextInPath = curTile;
        grid->tiles[curTile->i][curTile->j+1].dirToNextInPath = UP;
        grid->tiles[curTile->i][curTile->j+1].distFromExit = curTile->distFromExit + 1;

        //add to array so its adjacent blocks will be calculated
        grid->tiles[curTile->i][curTile->j+1].next = pathless;
        pathless = &grid->tiles[curTile->i][curTile->j+1];
      }
      //iterate to next tile and remove this one
      curTile = curTile->next;
    }

    //remove tiles that have been iterated through already
    curTile = pathless;
    while(curTile->next != prevTile && curTile->next != NULL){
      curTile = curTile->next;
    }
    if(curTile->next == NULL)
      pathless = NULL;
    else
      curTile->next = NULL;
  }
  printf("ticks used to calculate path: %i \n",SDL_GetTicks()-startTicks); //to get end time

  return grid->startTile->nextInPath != NULL; 
};

//calculates if adding a tower to 'selectedGrid' would block path
void wouldBlockPath(Grid *grid){
  if(grid->selectedTile->myTower == NULL){
    //Pretend like there is a tower there
    grid->selectedTile->myTower = NULL + 1;

    //finds out if there would still be a path and stores value
    grid->blocksPath = !findPath(grid);

    //reset to original value and resets path
    grid->selectedTile->myTower = NULL;
    findPath(grid);
  }else //If theres a tower, it blocks the path
    grid->blocksPath = TRUE;
};
