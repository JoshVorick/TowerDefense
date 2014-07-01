#include "enemyGenerator.h"

extern void initEnemy(Enemy *enemy, int type, Grid_Tile *startGrid, Sprite sprites[]);

void initEnemyGenerator(Game *game){
  int i,j,k;
  for(i=0;i<NUM_ENEMIES; i++)
    for(j=0; j<MAX_WAVES; j++)
      for(k=0; k<NUM_INFO_PIECES; k++)
        game->enemyGenerator.info[i][j][k] = 0;
  
  FILE *level;
  level = fopen("src/Game/Enemies/level1waves.txt", "r");
  
  fscanf(level, "%lf", &game->enemyGenerator.healthScale);
  fscanf(level, "%lf", &game->enemyGenerator.RGBScale);
  game->enemyGenerator.wave = 0;
  game->enemyGenerator.maxWave = MAX_WAVES;
  
  int loopAfter, nextWave;
  for(i=0;i<NUM_ENEMIES;i++){
    fscanf(level, "%d", &loopAfter);
    game->enemyGenerator.info[i][0][LOOP_AFTER] = loopAfter;
    fscanf(level, "%d", &nextWave);
    while(nextWave > 0){
      game->enemyGenerator.info[i][nextWave-1][LOOP_AFTER] = loopAfter;
      fscanf(level, "%d", &game->enemyGenerator.info[i][nextWave-1][START_DELAY]);
      fscanf(level, "%d", &game->enemyGenerator.info[i][nextWave-1][GROUP_DELAY]);
      fscanf(level, "%d", &game->enemyGenerator.info[i][nextWave-1][NUM_GROUPS]);
      fscanf(level, "%d", &game->enemyGenerator.info[i][nextWave-1][NUM_PER_GROUP]);
      fscanf(level, "%d", &game->enemyGenerator.info[i][nextWave-1][DELAY_WITHIN_GROUP]);
      fscanf(level, "%d", &nextWave);
    }
  }

  fclose(level);
  printf("Hi\n");
  for(i=0; i<NUM_ENEMIES; i++){
    loopAfter = game->enemyGenerator.info[i][0][LOOP_AFTER];
    printf("loops: %d \n", loopAfter);
    for(j=loopAfter; j<MAX_WAVES; j++){
      game->enemyGenerator.info[i][j][LOOP_AFTER] = game->enemyGenerator.info[i][j%loopAfter][LOOP_AFTER];
      game->enemyGenerator.info[i][j][START_DELAY] = game->enemyGenerator.info[i][j%loopAfter][START_DELAY];
      game->enemyGenerator.info[i][j][GROUP_DELAY] = game->enemyGenerator.info[i][j%loopAfter][GROUP_DELAY];
      game->enemyGenerator.info[i][j][NUM_GROUPS] = game->enemyGenerator.info[i][j%loopAfter][NUM_GROUPS];
      game->enemyGenerator.info[i][j][NUM_PER_GROUP] = game->enemyGenerator.info[i][j%loopAfter][NUM_PER_GROUP];
      game->enemyGenerator.info[i][j][DELAY_WITHIN_GROUP] = game->enemyGenerator.info[i][j%loopAfter][DELAY_WITHIN_GROUP];
    }
  }
/* 
  game->enemyGenerator.healthScale = 1.1;
  game->enemyGenerator.RGBScale = 0.9;
  game->enemyGenerator.wave = 0;
  game->enemyGenerator.maxWave = MAX_WAVES;
  
  int i,j;
  for(i=0;i<NUM_ENEMIES;i++){
    for(j=0; j<MAX_WAVES; j++){
      game->enemyGenerator.info[i][j][START_DELAY] = 180*i + 3;
      game->enemyGenerator.info[i][j][GROUP_DELAY] = 300 + 60*j;
      game->enemyGenerator.info[i][j][NUM_GROUPS] = 3 - i;
      game->enemyGenerator.info[i][j][NUM_PER_GROUP] = 3 + 1*i + j;
      game->enemyGenerator.info[i][j][DELAY_WITHIN_GROUP] = 60;// - 30*i;
      game->enemyGenerator.info[i][j][LOOP_AFTER] = MAX_WAVES;
    }
  }
*/ 
};

void addEnemies(Game *game){
  if(game->enemyGenerator.wave >= game->enemyGenerator.maxWave)
    return;  

  int doneSpawning = 0;//will get changed to NUM_ENEMIES-1 if all are done
  
  int i;
  for(i=0; i<NUM_ENEMIES; i++){
    //Checks to make sure this enemy type should spawn this wave
    if(game->enemyGenerator.info[i][game->enemyGenerator.wave][NUM_GROUPS] == 0)
      continue;

    //Check to see is an enemy of type i should spawn this tick
    int timeSinceStart= game->levelTime - game->enemyGenerator.info[i][game->enemyGenerator.wave][START_DELAY]-1;
    int cycleTime = game->enemyGenerator.info[i][game->enemyGenerator.wave][GROUP_DELAY] + game->enemyGenerator.info[i][game->enemyGenerator.wave][DELAY_WITHIN_GROUP] * game->enemyGenerator.info[i][game->enemyGenerator.wave][NUM_PER_GROUP];
    if(timeSinceStart > 0 && //Is it after thge delay time??
        timeSinceStart % cycleTime < cycleTime - game->enemyGenerator.info[i][game->enemyGenerator.wave][GROUP_DELAY] && //Is it during a time that isn't the delay period??
        (timeSinceStart % cycleTime) % game->enemyGenerator.info[i][game->enemyGenerator.wave][DELAY_WITHIN_GROUP] == 1 &&
        timeSinceStart < cycleTime * game->enemyGenerator.info[i][game->enemyGenerator.wave][NUM_GROUPS]) //Is the wave still going??
    {
      Enemy *newEnemy = malloc(sizeof(Enemy));
      initEnemy(newEnemy, ENEMY1 + i, game->grid->startTile, game->sprites);
    
      //add new enemy to end of list
      if(game->enemies == NULL)
        game->enemies = newEnemy;
      else{
        Enemy *curEnemy = game->enemies;
        while(curEnemy->nextEnemy != NULL){
          curEnemy = curEnemy->nextEnemy;
        }
        curEnemy->nextEnemy = newEnemy;
      }
    }else if(timeSinceStart > cycleTime * game->enemyGenerator.info[i][game->enemyGenerator.wave][NUM_GROUPS]){
      doneSpawning++;
    }
  }
  if(doneSpawning == NUM_ENEMIES - 1){
    game->levelTime = 0;
    game->enemyGenerator.wave++;
  }
};
