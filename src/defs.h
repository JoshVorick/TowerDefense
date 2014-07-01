#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_mixer.h"
#include "SDL/SDL_ttf.h"

#define SCREEN_WIDTH 500
#define SCREEN_HEIGHT 732

#define NUM_SPRITES_STARTMENU 1
#define NUM_SOUNDS_STARTMENU 0
#define NUM_KEYS_STARTMENU 5

enum startMenuSprites{
  sBACKGROUND
};

#define NUM_SPRITES_GAME 8 
#define NUM_SOUNDS_GAME 0
#define NUM_TOWERS 4
#define NUM_ENEMIES 2
#define NUM_GAME_KEYS 21
#define MAX_WAVES 50
#define NUM_INFO_PIECES 6

enum enemy_info{  //different values for each enemy type
  START_DELAY,        //ticks to delay before group of wave enters
  GROUP_DELAY,        //ticks to delay between sending each group
  NUM_GROUPS,         //num of groups this wave
  NUM_PER_GROUP,      //num enemies in each group
  DELAY_WITHIN_GROUP, //delay between each in consecutive enemy in a group
  LOOP_AFTER          //Number of waves before wave style starts looping
};

enum gameSprites{
  TRIANGLE,
  SQUARE,
  PENTAGON,
  HEXAGON,
  gBACKGROUND,
  GRID_TILE,
  ENEMY1,
  ENEMY2
};

enum directions{
  LEFT,
  DOWN,
  UP,
  RIGHT
};

enum gameKeys{
  H,
  J,
  K,
  L,
  LEFT_ARROW,
  DOWN_ARROW,
  UP_ARROW,
  RIGHT_ARROW,
  ONE,
  TWO,
  THREE,
  FOUR,
  Q,
  W,
  E,
  A,
  S,
  D,
  SHIFT,
  SPACE,
  RETURN
};

#define FALSE 0
#define TRUE 1

#define TRIANGLE_PRICE 300
#define TRIANGLE_SELL 230
#define TRIANGLE_FIRERATE 100 
#define TRIANGLE_DMG 15 
#define TRIANGLE_RANGE 150

#define SQUARE_PRICE 400
#define SQUARE_SELL 280
#define SQUARE_FIRERATE 30
#define SQUARE_DMG 20
#define SQUARE_RANGE 200

#define PENTAGON_PRICE 500
#define PENTAGON_SELL 400
#define PENTAGON_FIRERATE 30
#define PENTAGON_DMG 25 
#define PENTAGON_RANGE 250

#define HEXAGON_PRICE 600
#define HEXAGON_SELL 500
#define HEXAGON_FIRERATE 100
#define HEXAGON_DMG 300 
#define HEXAGON_RANGE 550


#define ENEMY1_SPEED 1.6
#define ENEMY1_HEALTH 300
#define ENEMY1_SCORE 100
#define ENEMY1_R 10
#define ENEMY1_G 5
#define ENEMY1_B 15

#define ENEMY2_SPEED 2
#define ENEMY2_HEALTH 600
#define ENEMY2_SCORE 150
#define ENEMY2_R 15
#define ENEMY2_G 20
#define ENEMY2_B 10
