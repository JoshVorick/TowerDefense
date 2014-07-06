#include "enemyGenerator.h"

extern void initEnemy(Enemy *enemy, int type, Grid_Tile *startGrid, Sprite sprites[]);

void initEnemyGenerator(EnemyGenerator *enemyGenerator){
  printf("init enemy generator start\n");
  enemyGenerator->levelTime = 0;

  int i,j,k;
  for(i=0;i<NUM_ENEMIES; i++)
    for(j=0; j<MAX_WAVES; j++)
      for(k=0; k<NUM_INFO_PIECES; k++)
        enemyGenerator->info[i][j][k] = 0;
  
  FILE *level;
  level = fopen("src/Game/Enemies/level1waves.txt", "r");
  
  fscanf(level, "%lf", &enemyGenerator->healthScale);
  fscanf(level, "%lf", &enemyGenerator->RGBScale);
  enemyGenerator->wave = 0;
  enemyGenerator->maxWave = MAX_WAVES;
  
  int loopAfter, nextWave;
  for(i=0;i<NUM_ENEMIES;i++){
    fscanf(level, "%d", &loopAfter);
    enemyGenerator->info[i][0][LOOP_AFTER] = loopAfter;
    fscanf(level, "%d", &nextWave);
    while(nextWave > 0){
      enemyGenerator->info[i][nextWave-1][LOOP_AFTER] = loopAfter;
      fscanf(level, "%d", &enemyGenerator->info[i][nextWave-1][START_DELAY]);
      fscanf(level, "%d", &enemyGenerator->info[i][nextWave-1][GROUP_DELAY]);
      fscanf(level, "%d", &enemyGenerator->info[i][nextWave-1][NUM_GROUPS]);
      fscanf(level, "%d", &enemyGenerator->info[i][nextWave-1][NUM_PER_GROUP]);
      fscanf(level, "%d", &enemyGenerator->info[i][nextWave-1][DELAY_WITHIN_GROUP]);
      fscanf(level, "%d", &nextWave);
    }
  }

  fclose(level);

  for(i=0; i<NUM_ENEMIES; i++){
    loopAfter = enemyGenerator->info[i][0][LOOP_AFTER];
    printf("loops: %d \n", loopAfter);
    for(j=loopAfter; j<MAX_WAVES; j++){
      enemyGenerator->info[i][j][LOOP_AFTER] = enemyGenerator->info[i][j%loopAfter][LOOP_AFTER];
      enemyGenerator->info[i][j][START_DELAY] = enemyGenerator->info[i][j%loopAfter][START_DELAY];
      enemyGenerator->info[i][j][GROUP_DELAY] = enemyGenerator->info[i][j%loopAfter][GROUP_DELAY];
      enemyGenerator->info[i][j][NUM_GROUPS] = enemyGenerator->info[i][j%loopAfter][NUM_GROUPS];
      enemyGenerator->info[i][j][NUM_PER_GROUP] = enemyGenerator->info[i][j%loopAfter][NUM_PER_GROUP];
      enemyGenerator->info[i][j][DELAY_WITHIN_GROUP] = enemyGenerator->info[i][j%loopAfter][DELAY_WITHIN_GROUP];
    }
  }
/* 
  enemyGenerator->healthScale = 1.1;
  enemyGenerator->RGBScale = 0.9;
  enemyGenerator->wave = 0;
  enemyGenerator->maxWave = MAX_WAVES;
  
  int i,j;
  for(i=0;i<NUM_ENEMIES;i++){
    for(j=0; j<MAX_WAVES; j++){
      enemyGenerator->info[i][j][START_DELAY] = 180*i + 3;
      enemyGenerator->info[i][j][GROUP_DELAY] = 300 + 60*j;
      enemyGenerator->info[i][j][NUM_GROUPS] = 3 - i;
      enemyGenerator->info[i][j][NUM_PER_GROUP] = 3 + 1*i + j;
      enemyGenerator->info[i][j][DELAY_WITHIN_GROUP] = 60;// - 30*i;
      enemyGenerator->info[i][j][LOOP_AFTER] = MAX_WAVES;
    }
  }
*/ 
  printf("init enemy generator end\n");
};

void generateEnemies(SubGame *subGame, Sprite sprites[NUM_SPRITES_GAME]){
  printf("gen enemies start\n");
  if(subGame->enemyGenerator->levelTime == 0)
    return;
  if(subGame->enemyGenerator->wave >= subGame->enemyGenerator->maxWave)
    return;  
  
  subGame->enemyGenerator->levelTime++;
  int doneSpawning = 0;//will get changed to NUM_ENEMIES-1 if all are done
  
  int i;
  for(i=0; i<NUM_ENEMIES; i++){
    //Checks to make sure this enemy type should spawn this wave
    if(subGame->enemyGenerator->info[i][subGame->enemyGenerator->wave][NUM_GROUPS] == 0)
      continue;

    //Check to see is an enemy of type i should spawn this tick
    int timeSinceStart = subGame->enemyGenerator->levelTime - subGame->enemyGenerator->info[i][subGame->enemyGenerator->wave][START_DELAY];
    int cycleTime = subGame->enemyGenerator->info[i][subGame->enemyGenerator->wave][GROUP_DELAY] + 
      subGame->enemyGenerator->info[i][subGame->enemyGenerator->wave][DELAY_WITHIN_GROUP] * subGame->enemyGenerator->info[i][subGame->enemyGenerator->wave][NUM_PER_GROUP];
    if(timeSinceStart > 0 && //Is it after thge delay time??
        timeSinceStart % cycleTime < cycleTime - subGame->enemyGenerator->info[i][subGame->enemyGenerator->wave][GROUP_DELAY] && //Is it during a time that isn't the delay period??
        (timeSinceStart % cycleTime) % subGame->enemyGenerator->info[i][subGame->enemyGenerator->wave][DELAY_WITHIN_GROUP] == 1 &&
        timeSinceStart < cycleTime * subGame->enemyGenerator->info[i][subGame->enemyGenerator->wave][NUM_GROUPS]) //Is the wave still going??
    {
      Enemy *newEnemy = malloc(sizeof(Enemy));
      initEnemy(newEnemy, ENEMY1 + i, subGame->grid->startTile, sprites);
    
      //add new enemy to end of list
      if(subGame->enemies == NULL)
        subGame->enemies = newEnemy;
      else{
        Enemy *curEnemy = subGame->enemies;
        while(curEnemy->next != NULL){
          curEnemy = curEnemy->next;
        }
        curEnemy->next = newEnemy;
      }
    }else if(timeSinceStart > cycleTime * subGame->enemyGenerator->info[i][subGame->enemyGenerator->wave][NUM_GROUPS]){
      doneSpawning++;
    }
  }
  if(doneSpawning == NUM_ENEMIES - 1){
    subGame->enemyGenerator->levelTime = 0;
    subGame->enemyGenerator->wave++;
  }
  printf("gen enemies end\n");
};
