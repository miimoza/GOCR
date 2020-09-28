#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <err.h>
#include <sys/types.h>
#include <dirent.h>
#include <SDL.h>
#include <SDL_image.h>
#include "pixel_operations.h"

struct Network
{
  size_t numInput;
  size_t numHidden;
  size_t numOutput;
  double accuracy;
  double maxAccuracy;

  double* inputs;
  double** ihWeights;
  double* hBiases;
  double* hOutputs;
  double* hOutputsA;

  double** hoWeights;
  double* oBiases;
  double* outputs;
  double* outputsA;

};

struct trainData
{
  size_t numItems;
  size_t maxItems;
  struct trainItem* items;
  const char* list;
  size_t* indexes;
  size_t currentIndex;
};

struct trainItem
{
  size_t numInput;
  size_t numOutput;
  double* input;
  double* target;
  char character;
};
struct Network new_Network(
              size_t numInput,
              size_t numHidden,
              size_t numOutput);

double getRandom(double a);
double sigmoid(double z);
double sigmoid_prime(double z);
double HyperTan(double x);
double* softmax(double *src, size_t size);
double** initMatrix(size_t height, size_t width, double initVal);
void freeMatrix(double ** mat, size_t height);
struct trainData shuffleData(struct trainData data);
struct trainItem* getNextData(struct trainData data);
struct trainData loadDataFromFolder(const char* path,const char* list);
double* imgToInput(SDL_Surface* img);
double* CharToOutput(char c, const char* list);
char imgToChar(struct Network network, SDL_Surface* img);
char outputToChar(double *output, const char* list);
void freeTrainData(struct trainData td);
void freeNetwork(struct Network nw);
void printbinimage(double* t);
void save_network(struct Network network, char * path);
struct Network test_network(struct Network network, struct trainData data);
struct Network forward(struct Network network, double* inputs);
struct Network load_network_from_file(char* path);
