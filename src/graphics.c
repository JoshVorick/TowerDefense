#include "graphics.h"

SDL_Surface *loadImage(char *name) {
  printf("load image start\n");
  //load image all fancy-shmancy-like
  printf("load image end\n");
  return IMG_Load(name); //SDL_DisplayFormat(IMG_Load(name));  
};

void drawString(char *text, int x, int y, TTF_Font *font, int centerX, int centerY, SDL_Color foregroundColor, SDL_Color backgroundColor){
  printf("draw string start\n");
  SDL_Rect dest;
  SDL_Surface *surface = TTF_RenderUTF8_Shaded(font, text, foregroundColor, backgroundColor);
  if(surface == NULL)
    printf("KABOOOOOOM!!!\n");

  dest.x = (centerX == 1 ? (screen->w - surface->w) / 2 : x);
  dest.y = (centerY == 1 ? (screen->h - surface->h) / 2 : y);
  dest.w = surface->w;
  dest.h = surface->h;

  SDL_BlitSurface(surface, NULL, screen, &dest);

  SDL_FreeSurface(surface);
  printf("draw string end\n");
};

void drawImage(SDL_Surface *image, int x, int y){
  printf("draw image start\n");
  SDL_Rect dest;
  dest.x = x;
  dest.y = y;
  dest.w = image->w;
  dest.h = image->h;

  SDL_BlitSurface(image, NULL, screen, &dest);
  printf("draw image end\n");
};
