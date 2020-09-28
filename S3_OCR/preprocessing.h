# include <stdlib.h>
# include <SDL.h>


void getHisto(SDL_Surface * img, int histo[]);

void setGreyscale(SDL_Surface *img);

int getTreshold(int histo[], int size_x, int size_y);

void toBinary(SDL_Surface *img);

SDL_Surface* setContrast(SDL_Surface *surface);

SDL_Surface* setNoise(SDL_Surface *surface);

SDL_Surface* setRotation(SDL_Surface *surface);
