#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/dir.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "helper.h"

/*Fonctions de chaines :
	strcpy() --> copie une chaine de caractere dans une autre
	strlen() --> longueur de la chaîne
	strcmp() --> comparer 2 chaînes de caractères


*/


//Cree un document et check d'abord le système d'exploration
void make_directory(const char* name)
   {
   #ifdef __linux__
       mkdir(name, 0777);
   #else
       _mkdir(name);
   #endif
   }


void reverseString(char *a,char *b)
{
char temp= ' ';
	while(a < b)
	{
		temp= *a;
		*a= *b;
		*b= temp;
		a++; b--;
	}
}

char *int2string (int n)
{
  int x = 0;
  char c = ' ';
  int space=1,pos =0;
  char *res = NULL;

  res = (char *) malloc (sizeof(char) * (space+1 +1));

  while (n>=10)
   {
	x = n%10;
	c = x + '0';
	res[pos]= c;
	pos++;
	n = n/10;
   }

   res[pos] =n + '0';
   res[pos+1] = '\0';
   reverseString(res, res+strlen(res)-1);
   return res;
}
