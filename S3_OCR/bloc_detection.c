# include "bloc_detection.h"
# include "pixel_operations.h"
# include "helper.h"
# include <limits.h>
#include <math.h>


// Catch all line in a surface, create a folder with the number of the line
// And put all chars.jpg in
void CatchAndSave(SDL_Surface *img, char res[], struct Network network){
	int num_line = 0;
	int num_char;
        //char *res = malloc(sizeof(char)*200);
	char * path_char = malloc(sizeof(char) * 200);
	char * path_line = malloc(sizeof(char) * 200);

  	make_directory("../tmp/");
  	printf("created \n");
  int i=0;
	while(!isAllGreen(img)){
		SDL_Surface* line = catchLine(img);
		display_image(line);
		wait_for_keypressed();
  	        white_to_green_vertical(line);
  	        display_image(line);
  	         wait_for_keypressed();
                detect_spaces(line);
                display_image(line);
		wait_for_keypressed();

	        memset(path_line, '\0', 200);
		sprintf(path_line, "../tmp/Line_%i", num_line);
		//printf("at the begining : %s \n", path_line);
                make_directory(path_line);

		num_char = 0;
		strcat(res, "\n"); //saut de ligne

		while(!isAllGreen(line))
		{
			SDL_Surface* chr = catchChr(line);
			//chr = Crop(chr);
			chr = ScaleSurface(chr, 32);
			chr = Center(chr,32,32);
			//resize(chr);
			//display_image(chr);
			//wait_for_keypressed();
			memset(path_char, '\0', 200);
			sprintf(path_char,"../tmp/Line_%i/Char_%i.bmp", num_line, num_char);
			SDL_SaveBMP(chr, path_char);
			//printf("path char = %s \n", path_char);

			if (is_space(chr))
                        {
                           res[i] = ' ';
                        }
                        else
                        {
                          char c = imgToChar(network, chr);

                    //      printf("char %c\n", c);
                          res[i] = c;
                          //strcat(res,c); //res+=c
                        ///  printf("%s\n",res );
                          //display_image(chr);
                          //wait_for_keypressed();
                          //display_image(line);
                          //wait_for_keypressed();
                          SDL_FreeSurface(chr);
                          num_char ++;
                        }
		i++;
		}
		res[i] = '\n';
		i++;
    		//printf("End of the line.\n");
		num_line++;
		SDL_FreeSurface(line);
	}
	free(path_line);
	free(path_char);
	//printf("End of the page\n");
	//return res;
}


int is_space (SDL_Surface *chr)
{
  for(double w =0;w<chr->w;w++)
  {
   for(double h=0; h<chr->h;h++)
   {
     Uint8 r, g, b;
     SDL_GetRGB(getpixel(chr, w, h), chr->format, &r, &g, &b);
     if(!r || !g || !b)
       return 0;
   }
  }
  return 1;
}

/*Regarde si besoin de rotation ou pas */
/*int SDL_Need_Rotation(SDL_Surface *image)
{
  //int res = 1;
  double w = 0;
  double h = 0;
  //Uint32 couleur;
  //Uint8 average_colX = average_on_cols(image,0);
  //Uint8 average_colY = average_on_line(image,0);

  for(;w<image->w && is_black(image, w, 0); w++);
  for(;h<image->h && is_black(image, 0, h); h++);
  printf("w = %lf h = %lf \n",w,h);

  Uint8 average_on_cols_ = average_on_cols(image, h);
  Uint8 average_on_lines_ = average_on_line(image, w);
  if(average_on_cols_ > average_on_lines_)
    return 1;
  return 0;
}


SDL_Surface* Get_inteligne(SDL_Surface *img)
{
  double h;
  double w;
  double X1;
  for(;w<img->w && !is_black(img, w, 0); w++);
  X1 = w;
  for(;h<image->h && is_black(image, 0, h); h++);


}

int is_black(SDL_Surface *img,double w,double h)
{
	Uint8 r, g, b;
	SDL_GetRGB(getpixel(img, w, h), img->format, &r, &g, &b);
	return (!r && !g && !b);
}*/

/*effectue une rotation centrale */
SDL_Surface* SDL_RotationCentralN(SDL_Surface* origine, float angle)
{
 SDL_Surface* destination;
 int i;
 int j;
 Uint32 couleur;
 int mx, my, mxdest, mydest;
 int bx, by;
 float angle_radian;
 float tcos;
 float tsin;
 double largeurdest;
 double hauteurdest;

/* détermine la valeur en radian de l'angle*/
 angle_radian = -angle * M_PI / 180.0;

/*pour éviter pleins d'appel, on stocke les valeurs*/
 tcos = cos(angle_radian);
 tsin = sin(angle_radian);

/*calcul de la taille de l'image de destination*/
 largeurdest=   ceil(origine->w * fabs(tcos) + origine->h * fabs(tsin)),
 hauteurdest=   ceil( origine->w * fabs(tsin) + origine->h * fabs(tcos)),
/* fabs <=> val abs
 largeurdest = orgin->h quand alpha = 90
ceil() retourne l'entier strictement suppérieur au flottant */


 destination = SDL_CreateRGBSurface(SDL_HWSURFACE, largeurdest, hauteurdest, origine->format->BitsPerPixel,
			origine->format->Rmask, origine->format->Gmask, origine->format->Bmask, origine->format->Amask);

 /*on vérifie que la mémoire a été allouée*/
 if(destination==NULL)
  return NULL;

 /*calcul du centre des images*/
 mxdest = destination->w/2.;
 mydest = destination->h/2.;
 mx = origine->w/2.;
 my = origine->h/2.;

 /*compteurs pour les bornes de destination*/
 for(j=0;j<destination->h;j++)
  for(i=0;i<destination->w;i++)
  {
   /* Commpteur pour les bornes de origine*/
   bx = (ceil (tcos * (i-mxdest) + tsin * (j-mydest) + mx));
   by = (ceil (-tsin * (i-mxdest) + tcos * (j-mydest) + my));

   if (bx>=0 && bx< origine->w && by>=0 && by< origine->h)
   {
     couleur = getpixel(origine, bx, by);
     putpixel(destination, i, j, couleur);
   }
 }

return destination;
}


void detect_spaces(SDL_Surface *line)
{
	int size_treshold = (int) average_size_char(line) / 1.5;
	//printf("average = %i\n",size_treshold);
	double w = 0;
        int size;
	for(; w<line->w && is_green(line, w, 0); w++);
	while(w < line->w){
        	size = 0;
		while(w < line->w && is_green(line, w, 0) && size < size_treshold){
 			w++;
			size++;
		}

                if(size >= size_treshold){
			if(w-2 >= 0)
				color_zone(line, SDL_MapRGB(line->format, 255, 0, 0), w-2, 0, w-1, line->h);
                        for(; w<line->w && is_green(line, w, 0); w++);
                }

        	size = 0;
        	while(w < line->w && !is_green(line, w, 0)){
			w++;
			size++;
		}
	}

}

double average_size_char(SDL_Surface *line){
	double sum = 0;
	double nb_char = 0;
	double w = 5;
	double X1;
	while(w<line->w){
		for(;w<line->w && is_green(line, w, 0); w++);
		X1 = w;
	        for(; w<line->w && !is_green(line, w, 0); w++);
		sum += (w - X1);
		nb_char++;
	}
	return sum/nb_char;
}


SDL_Surface* catchLine(SDL_Surface *img){
	double y1 = getY1(img);
	double y2 = getY2(img,y1);
	SDL_Surface* line = catchZone(img, 0, img->w, y1, y2);
	color_zone(img, SDL_MapRGB(img->format, 0, 255,0), 0, y1 , img->w, y2);
	return line;
}

SDL_Surface* catchChr(SDL_Surface *line)
{
	double x1 = getX1(line);
	double x2 = getX2(line,x1);
	SDL_Surface* chr = catchZone(line, x1, x2, 0, line->h);
	color_zone(line, SDL_MapRGB(line->format, 0, 255, 0), x1, 0, x2, line->h);
	return chr;
}

//Check if the surface is all green
int isAllGreen(SDL_Surface *img){
	int isNotAllGreen = 0;
	for(double h = 0;  h < img->h; h++)
    		for(double w = 0;  w < img->w; w++)
			isNotAllGreen += !is_green(img, w,h);
	return !isNotAllGreen;
}

void white_to_green_horizontal(SDL_Surface *img)
{
	// horizontal fill
	for(double h=0; h< img->h; h++)
	{
	//	Uint8 color_pixel = average_on_line(img, h);
		int t = 0;
		for(int w = 0; w<img->w; w++)
		{
			Uint8 p;
			SDL_GetRGB(getpixel(img, w, h), img->format, &p, &p, &p);
			if(p == 0)
			{
				t++;
			}
		}
		if(t == 0)
		{
			color_zone(img, SDL_MapRGB(img->format, 0, 255,0), 0, h, img->w, h);
		}
		/*
		if(color_pixel > 253)
		{
			//printf("before color zone %d\n", h);
			color_zone(img, SDL_MapRGB(img->format, 0, 255,0), 0, h, img->w, h);
			//printf("after %d\n", h);
		}
		*/
	}
}



void white_to_green_vertical(SDL_Surface *line){
	//vertical fill
	for(double w=0; w< line->w; w++)
	{
		Uint8 color_pixel = average_on_cols(line, w);
		if(color_pixel > 253)
		{
			//printf("before color zone %d \n",w);
			color_zone(line, SDL_MapRGB(line->format, 0, 255,0), w, 0, w, line->h);
			//printf("after %d\n", w);
		}
	}
}



SDL_Surface* catchZone(SDL_Surface *img,
	 										double x1, double x2, double y1, double y2){
	SDL_Rect position;
	SDL_Surface *rectangle;
	 // Create empty surface with good dimensions
	rectangle = SDL_CreateRGBSurface(SDL_HWSURFACE, x2-x1, y2-y1, 32, 0, 0, 0, 0);

	position.x = x1;
	position.y = y1;
	position.w = x2;
	position.h = y2;
	SDL_BlitSurface(img,&position,rectangle,NULL);
	return rectangle;
}

double getX1(SDL_Surface *line){
	double w = 0;
	while(w < line->w && is_green(line, w, 0))
		w++;
	return w;
}

double getX2(SDL_Surface *line, double w){
	while(w < line->w && !is_green(line, w, 0))
		w++;
	return w;
}

double getY1(SDL_Surface *img){
	double h = 0;
	while(h < img->h && is_green(img, 0, h))
		h++;
	return h;
}

double getY2(SDL_Surface *img, double h){
	while(h < img->h && !is_green(img, 0, h))
		h++;
	return h;
}

int is_green(SDL_Surface *img,double w,double h)
{
	Uint8 r, g, b;
	SDL_GetRGB(getpixel(img, w, h), img->format, &r, &g, &b);
	return (!r && g && !b);
}

// Color a zone, with a pixel given, zone delimited by x1,x2,y1,y2,
//including all the extrimities in the zone, take a surface and modify it
void color_zone(SDL_Surface *img, Uint32 pixel,
		double x1, double y1, double x2, double y2)
{
  for(double h = y1; h<img->h && h <= y2; h++)
    for(double w = x1;w<img->w &&  w <= x2; w++)
      putpixel(img, w, h, pixel);
}

// Return the pixel average on a line
Uint8 average_on_line(SDL_Surface *img, double line){
  double sum_value = 0;
  double nb_pixel = img->w; // calcul the area
	// We work in black and white, then we take only one grey level value
  Uint8 grey_lvl;

	double w = 0.0;
	while(w<(img->w))
	{

		SDL_GetRGB(getpixel(img, w, line), img->format, &grey_lvl, &grey_lvl, &grey_lvl);
    sum_value += grey_lvl;
		w++;
	}

  return sum_value / nb_pixel;
}


// Return the pixel average on a cols
Uint8 average_on_cols(SDL_Surface *img, double cols){
  double sum_value = 0;
  double nb_pixel = img->h; // calcul the area
	// We work in black and white, then we take only one grey level value
  Uint8 grey_lvl;
//	printf("max w %f max h %f\n", img->w, img->h );
	double h =0.0;
	while(h< (img->h))
	{
		SDL_GetRGB(getpixel(img, cols, h), img->format,&grey_lvl, &grey_lvl, &grey_lvl);
    sum_value += grey_lvl;
		h++;
	}

	printf("\n" );
  return sum_value / nb_pixel;
}

SDL_Surface *ScaleSurface(SDL_Surface *img, int bound)
{
  int width;
  int height;
	Uint32 pixel;

  float scale = (float) bound / (float) img->h;
  height = (float)img->h * scale;
  width = (float)img->w * scale;

  if(height > bound || width > bound)
  {
    float scale = (float) bound / (float) img->w;
    height = (float)img->h * scale;
    width = (float)img->w * scale;
  }

  SDL_Surface *surface = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);

  for(double h = 0;  h < height; h++)
  {
    for(double w = 0;  w < width; w++)
    {
      pixel = getpixel(img,
                        (int)((float) w / (float) width  * (float)img->w),
                        (int)((float) h / (float) height * (float)img->h));
      putpixel(surface,w,h,pixel);
    }
  }

  return surface;
}

SDL_Surface *Center(SDL_Surface *img, int width, int height)
{

	Uint32 pixel = SDL_MapRGB(img->format, 255, 255, 255);
	SDL_Surface *surface = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);
	for(double h = 0;  h < height; h++)
  {
    for(double w = 0;  w < width; w++)
    {
      putpixel(surface,w,h,pixel);
    }
  }

	int marginleft = (width - img->w) / 2;
	int margintop = height - img->h;

	for(double h = 0;  h < img->h; h++)
  {
    for(double w = 0;  w < img->w; w++)
    {
      pixel = getpixel(img,w,h);
      putpixel(surface,marginleft + w,margintop + h,pixel);
    }
  }
	return surface;
}

SDL_Surface *Crop(SDL_Surface *img)
{
	if(img->w < 5)
		return img;
	double l1 = 0;
	double l2 = img->h - 1;

	double c1 = 0;
	double c2 = img->w - 1;

	Uint32 pixel;
	while(l1 < img->h && average_on_line(img,l1) == 255)
	{
		l1++;
	}
	while(l2 > 0 && average_on_line(img,l2) == 255)
	{
				l2--;
	}
	while(c1 < img->w && average_on_cols(img,c1) == 255)
	{
		c1++;
	}
	while(c2 > 0 && average_on_cols(img,c2) == 255)
	{
		c2--;
	}

	SDL_Surface *surface = SDL_CreateRGBSurface(0, c2-c1, l2-l1, 32, 0, 0, 0, 0);
	for(double h = 0;  h < l2-l1; h++)
  {
    for(double w = 0;  w < c2-c1; w++)
    {
			pixel = getpixel(img,w+c1,h+l1);
      putpixel(surface,w,h,pixel);
    }
  }
	free(img);
	//display_image(surface);
	//wait_for_keypressed();
	return surface;
}
