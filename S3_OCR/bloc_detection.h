//SDL_Surface* textBlocDetection(SDL_Surface *surface);
//SDL_Surface* lineSplit(SDL_Surface *surface);
//SDL_Surface* caracterSplit(SDL_Surface *surface);


# include <err.h>
# include <SDL.h>
# include <SDL_image.h>
# include "neural_network.h"


void CatchAndSave(SDL_Surface *img, char res[], struct Network network);
int is_space (SDL_Surface *chr);
void detect_spaces(SDL_Surface *img);
double average_size_char(SDL_Surface *img);
int isAllGreen(SDL_Surface *img);
void white_to_green_horizontal(SDL_Surface *img);
void white_to_green_vertical(SDL_Surface *line);
SDL_Surface* catchChr(SDL_Surface *line);
double getX1(SDL_Surface *line);
double getX2(SDL_Surface *line, double w);
SDL_Surface* catchLine(SDL_Surface *img);
SDL_Surface* catchZone(SDL_Surface *img,
                      double x1, double x2, double y1, double y2);
double getY1(SDL_Surface *img);
double getY2(SDL_Surface *img, double h);
int is_green(SDL_Surface *img,double w,double h);
void color_zone(SDL_Surface *img, Uint32 pixel,
                      double x1, double y1, double x2, double y2);
Uint8 average_on_line(SDL_Surface *img, double cols);
Uint8 average_on_cols(SDL_Surface *img, double cols);
SDL_Surface *ScaleSurface(SDL_Surface *img, int bound);
SDL_Surface *Center(SDL_Surface *img, int width, int height);
SDL_Surface *Crop(SDL_Surface *img);
SDL_Surface* SDL_RotationCentralN(SDL_Surface* origine, float angle);
//int SDL_Need_Rotation(SDL_Surface *image);
//int is_black(SDL_Surface *img,double w,double h);
