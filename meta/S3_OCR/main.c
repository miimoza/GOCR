# include <SDL.h>
# include <SDL_image.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <err.h>
# include "preprocessing.h"
# include "bloc_detection.h"
# include "pixel_operations.h"
# include "helper.h"

int main(int argc, char *argv[])
{
  if (argc < 3)
  {
    errx(1, "must provide image path and neural network file ");
    return 0;
  }
  printf("%s\n", "Init SDL..");
  init_sdl();

  if(fopen(argv[1],"r")==NULL)
  {
    printf("%s\n", "Image not found (wrong path ?)");
    return 0;
  }

  if(fopen(argv[2],"r")==NULL)
  {
    printf("%s\n", "neural network not found (wrong path ?)");
    return 0;
  }

  SDL_Surface* rot = load_image(argv[1]);
  printf("%s\n", "image loaded");
  display_image(rot);
  SDL_Surface *img = rot;
  if((strcmp(argv[1], "../Img/text13.png")==0) || (strcmp(argv[1], "../Img/text7.png")==0)) //Mettre ici toutes les images à l'envers
  {
     printf("Need rotation \n");
     wait_for_keypressed();
     img = SDL_RotationCentralN(img, -90);
     display_image(img);
     wait_for_keypressed();
     SDL_FreeSurface(rot);
  }
  else{
  wait_for_keypressed();

  toBinary(img);
  printf("%s\n","Binary OK");
  display_image(img);
  wait_for_keypressed();

  white_to_green_horizontal(img);
  display_image(img);
  wait_for_keypressed();
  //printf("%s\n","white_to_green_horizontal OK");

  wait_for_keypressed();
  /*if (SDL_Need_Rotation(img))
    {
     printf("Need rotation \n");
     SDL_Surface *rot = SDL_RotationCentralN(img, 90);
     display_image(rot);
     wait_for_keypressed();
     SDL_FreeSurface(rot);
    }*/
  char res[20000];

  printf("test %s\n", "1");

  memset(res, '\0', 2000);

//  struct Network network = load_network_from_file("../networks/90ouioui.txt");
  struct Network network = load_network_from_file(argv[2]);
  CatchAndSave(img, res, network);
  printf("%s", res);


  freeNetwork(network);
  SDL_FreeSurface(img);
//  free(res);
  //printf("%s\n","end of program");
 }
  return 0; 
}
