# include "preprocessing.h"
# include "pixel_operations.h"
# include <err.h>
# include <SDL.h>
# include <SDL_image.h>
# include <math.h>


// get the Histo array in a zone where (x1, y1) is le top left corner
// (x2,y2) is the bottom right corner, taking the list histo[] in reference
void getHistoInZone(SDL_Surface *img, int histo[],
                  int x1, int y1, int x2, int y2)
{
  Uint32 pixel;
  Uint8 n;
  for(int i=x1;i<x2;i++)
  {
    for(int j=y1;j<y2;j++)
    {
      pixel = getpixel(img, i, j);
      SDL_GetRGB(pixel, img->format, &n, &n , &n);
      histo[n]+=1;
    }
  }
}

// Return the treshold, index which divises the list depended on the histogram.
int getTreshold(int histo[], int x_size, int y_size){

  /* calculation of probability density */
  double prob[256];
  for(int i =0;i<256; i++)
    prob[i] = (double) ( (double)(histo[i]) / (x_size * y_size));

  /* omega & myu generation */
  double omega[256];
  double mu[256];

  omega[0] = prob[0];
  mu[0] = 0.0;
  for(int i=1;i<256;i++)
  {
    omega[i] = omega[i-1] + prob[i];
    mu[i] = mu[i-1] + i*prob[i];
  }

  /* sigma */
  int treshold = 0;
  double max_sigma = 0.0;
  double sigma[256];
  for(int i = 0;i<255; i++)
  {
    if(omega[i] != 0.0 && omega[i] != 1.0)
      sigma[i] = pow(mu[255]*omega[i] - mu[i],2) / (omega[i]*(1.0 - omega[i]));
    else
      sigma[i] = 0.0;

    if(sigma[i] > max_sigma) {
      max_sigma = sigma[i];
      treshold = i;
    }
  }

  return treshold;

}

//apply the ostu method in a zone where (x1, y1) is le top left corner
// and (x2,y2) is the bottom right corner
void toBinaryInZone(SDL_Surface *img,
                    int treshold, int x1, int y1, int x2, int y2)
{
  Uint32 pixel;
  Uint8 n;

  for(int i=x1;i<x2;i++){
    for(int j=y1;j<y2;j++){
      pixel = getpixel(img, i, j);
      SDL_GetRGB(pixel, img->format, &n, &n, &n);
      if( n < treshold)
        pixel = SDL_MapRGB(img->format, 0, 0, 0); // all black
      else
        pixel = SDL_MapRGB(img->format, 255, 255, 255); // all white

      putpixel(img, i, j, pixel);
    }
  }

}


//Put the picture in black and white, divide the image in rectangles,
//then apply the otsu method for each rectangle.
void toBinary(SDL_Surface *img)
{
  setGreyscale(img);
  int histo[256];

  // Define the blocs width size and height size
  int bloc_size_x = (img->w) / 12 ;
  while(img->w % bloc_size_x != 0 && bloc_size_x < (img->w) / 4)
    bloc_size_x++; // Increase while not modulo

  int bloc_size_y = (img->h) / 12;
  while(img->h % bloc_size_y != 0 && bloc_size_y < (img->h) / 4)
    bloc_size_y++; // Increase while not modulo


  int treshold;
  for(int h = 0;  h < img->h / bloc_size_y; h++)
    for(int w = 0;  w < img->w / bloc_size_x; w++)
    {
      // Init the histo we'll modify
      for(int i=0;i<256;i++)
        histo[i] = 0;

      getHistoInZone(img, histo,
                    w*bloc_size_x, h*bloc_size_y, w*bloc_size_x+bloc_size_x,
                    h*bloc_size_y+bloc_size_y);
      treshold = getTreshold(histo, bloc_size_x, bloc_size_y);

      //Technic make the treesold more efficient

      // If the treshold is too high (meaning the bloc is globally white),
      if(treshold > 165)
        treshold = 150; // We down the treshold to make the result more white

      // If the treshold is too high (mening the bloc is globally white),
      if(treshold > 165)
        treshold = 150; // We down the treshold to make the result more white

      toBinaryInZone(img, treshold,
                    w*bloc_size_x, h*bloc_size_y, w*bloc_size_x+bloc_size_x,
                    h*bloc_size_y+bloc_size_y);
    }
}


// Take a picture and return it in black and white
void setGreyscale(SDL_Surface *img)
{
  Uint32 pixel;
  Uint8 r;
  Uint8 g;
  Uint8 b;
  float n;

  for(double h = 0;  h < img->h; h++)
    for(double w = 0;  w < img->w; w++)
    {
      pixel = getpixel(img, w, h);
      SDL_GetRGB(pixel, img->format, &r, &g , &b);
      n = 0.3 * (float)r + 0.59 * (float)g + 0.11 * (float)b;
      //n = 0.2 * (float)r + 0.79 * (float)g + 0.01 * (float)b;
      r = (Uint8) n;
      g = (Uint8) n;
      b = (Uint8) n;

      pixel = SDL_MapRGB(img->format, r, g, b);
      putpixel(img, w, h, pixel);
    }
}

SDL_Surface* setRotation(SDL_Surface *surface)
{
  return surface;
}
