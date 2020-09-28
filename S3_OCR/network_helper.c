#include "network_helper.h"

double randfrom(double min, double max)
{
    double range = (max - min);
    double div = RAND_MAX / range;
    return min + (rand() / div);
}

double getRandom(double a)
{
  //return (double)rand()/RAND_MAX*(2.0 * (1.0 / sqrt(a))) - (1.0 / sqrt(a));
  return randfrom(-(1/sqrt(a)), (1/sqrt(a)));
}



double sigmoid(double z)
{
  return 1.0 / (1.0 + exp(-z));
}

double sigmoid_prime(double z)
{
  return sigmoid(z) * (1 - sigmoid(z));
}

double HyperTan(double x)
{
  if(x<-20.0) return -1.0;
  else if (x > 20.0) return 1.0;
  else return tanh(x);
}

double* softmax(double *src, size_t size)
{
  double* dest = malloc(sizeof(double) * size);
  double sum = 0.0;
  for(size_t j = 0; j<size;j++)
  {
    for(size_t k = 0; k<size; k++)
      sum += exp(src[k]);
    dest[j] = (exp(src[j]) / sum);
    sum = 0.0;
  }
  free(src);
  return dest;
}

struct trainData shuffleData(struct trainData data)
{
  data.currentIndex = 0;

  size_t n = data.numItems;
  if(n > 1)
  {
    for(size_t i = 0; i<n;i++)
    {
      size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
      int t = data.indexes[j];
      data.indexes[j] = data.indexes[i];
      data.indexes[i] = t;
    }
  }
  return data;

}

struct trainItem* getNextData(struct trainData data)
{

  if(data.currentIndex >= data.numItems)
    return NULL;
  return &data.items[data.indexes[data.currentIndex]];
}


double** initMatrix(size_t height, size_t width, double initVal)
{
  double** mat = malloc(sizeof(double*) * height);
  for(size_t i = 0;i<height;i++){
    mat[i] = malloc(sizeof(double) * width);
    for(size_t j = 0; j<width;j++){
      mat[i][j] = initVal;
    }
  }
  return mat;
}

void freeMatrix(double ** mat, size_t height)
{
  for(size_t i=0;i<height; i++)
    free(mat[i]);
  free(mat);
}

struct trainData addItem(struct trainData trainData, struct trainItem item)
{

  trainData.numItems++;
  if(trainData.numItems < trainData.maxItems)
  {

    trainData.items[trainData.numItems - 1] = item;
    free(trainData.indexes);
    trainData.indexes = malloc(sizeof(size_t) * trainData.numItems);
    for(size_t i = 0; i<trainData.numItems; i++)
      trainData.indexes[i] = i;
  }
  return trainData;
}

struct trainData initData(struct trainData td)
{
  td.maxItems = 2000;
  td.numItems = 0;
  td.currentIndex = 0;
  td.items = malloc(sizeof(struct trainItem) * td.maxItems);
  td.indexes = malloc(sizeof(size_t) * td.numItems);
  return td;
}

void freeTrainData(struct trainData td)
{
  free(td.indexes);
  for(size_t i = 0; i<td.numItems; i++)
  {
    struct trainItem item = td.items[i];
    free(item.input);
    free(item.target);

  }
  free(td.items);

}

struct trainData loadDataFromFolder(const char* path, const char* list)
{
  size_t pathlen = strlen(path);
  size_t listlen = strlen(list);
  struct trainData td;
  td = initData(td);
  td.list = list;
  char* imgpath = malloc((pathlen + 20) * sizeof(const char*));
  DIR* rep;
  struct dirent* lecture;

  for(size_t i = 0; i<listlen;i++)
  {
      memset(imgpath, '\0', 100);
      strcpy(imgpath, path);
      *(imgpath + pathlen) = list[i];
      *(imgpath + pathlen + 1) = '/';

      rep = opendir(imgpath);
      if(rep){
        while ((lecture = readdir(rep)))
        {
          if(lecture->d_name[4] > 4)
          {
            strcpy((imgpath + pathlen + 2), lecture->d_name);
            SDL_Surface *img = SDL_LoadBMP(imgpath);
            struct trainItem item;
            item.numInput = 1024;
            item.numOutput = listlen;
            item.input = imgToInput(img);
            item.target = CharToOutput(list[i], list);
            item.character = list[i];
            td = addItem(td, item);

            SDL_FreeSurface(img);
          }
        }
      } else {
        printf("unknown file or directory : %s\n",imgpath );
      }

      closedir(rep);
  }
  free(imgpath);

/*
  for(size_t i = 0; i<td.numItems; i++)
  {
    printf("items: %zu\n",i );
    printbinimage(td.items[i].input);
  }
  */

  return td;
}

char imgToChar(struct Network network, SDL_Surface* img)
{
  double* input = imgToInput(img);
  network = forward(network, input);
  char c = outputToChar(network.outputs, "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ'");
  free(input);
  return c;
}

double* imgToInput(SDL_Surface* img)
{
  double* t = malloc(sizeof(double) * 1024);
  Uint32 pixel;
  for(int h = 0; h < img->h;h++)
  {
    for(int w = 0; w < img->w;w++)
    {
      pixel = getpixel(img,w,h);
      *(t +h * 32 + w) = ((int) pixel == 0 ? 1.0 : 0.0);
    }
  }
  return t;
}

double* CharToOutput(char c, const char* list)
{
  double *out = malloc(sizeof(double) * strlen(list));
  double *begin = out;
  for(;begin< out + strlen(list);begin++)
    *begin = 0;

  int i = 0;
  while(*list != c)
  {
    list++;
    i++;
  }

  *(out + i) = 1;
  return out;
}

void freeNetwork(struct Network nw)
{
  freeMatrix(nw.hoWeights, nw.numHidden);
  freeMatrix(nw.ihWeights, nw.numInput);

  free(nw.inputs);

  free(nw.hBiases);
  free(nw.hOutputs);
  free(nw.outputs);
  free(nw.hOutputsA);

  free(nw.oBiases);
  //free(nw.outputs);
  free(nw.outputsA);
}

void printbinimage(double* t)
{
  for(int i = 0; i<32; i++)
  {
    for(int j = 0; j<32; j++)
    {
      printf("%d ", (int)t[i * 32 + j]);
    }
    printf("\n" );
  }

  printf("%s\n","\n\n\n\n" );
}

char outputToChar(double *output, const char* list)
{
/*
  for (size_t i = 0; i < strlen(list); i++) {
  printf("%f\n", output[i]);
  }
*/
  size_t maxindex = 0;
  double *maxptr = output;
  double *cur = output;
  for (size_t i = 0; i < strlen(list); i++) {
    if(*cur > *maxptr)
      maxptr = cur;
    cur++;
  }
  maxindex = maxptr - output;
  return list[maxindex];
}

void save_network(struct Network network, char * path)
{
  FILE *file;
  file = fopen(path, "wb+");
  if(file == NULL) //if file does not exist, create it
  {
      file = fopen(path, "wb");
  }
  fprintf(file, "%zu\n", network.numInput);
  fprintf(file, "%zu\n", network.numHidden);
  fprintf(file, "%zu\n", network.numOutput);
  for(size_t i=0; i<network.numHidden;i++)
    fprintf(file,"hBiases[%zu] = %f\n",i,network.hBiases[i]);


  for(size_t i=0; i<network.numInput;i++)
    for(size_t j=0; j<network.numHidden;j++)
      fprintf(file,"ihWeights[%zu][%zu] = %f\n",i,j,network.ihWeights[i][j]);

  for(size_t i=0; i<network.numOutput;i++)
    fprintf(file,"oBiases[%zu] = %f\n",i,network.oBiases[i]);

  for(size_t i=0; i<network.numHidden;i++)
    for(size_t j=0; j<network.numOutput;j++)
      fprintf(file,"hoWeights[%zu][%zu] = %f\n",i,j,network.hoWeights[i][j]);

  fclose(file);
  printf("%s\n","network saved" );
}

struct Network test_network(struct Network network, struct trainData data)
{
  char pre = 'a';
  double numTot = 0.0;
  double numErr = 0.0;
  double totReussite = 0.0;
  for(size_t i=0; i<data.numItems;i++)
  {
    struct trainItem item = data.items[i];

    if(pre != item.character)
    {
      printf("%c : %f‰ \n", pre, 100.0 - ((numErr / numTot) * 100.0) );
      totReussite = totReussite + 100.0 - ((numErr / numTot) * 100.0);
      numTot = 0;
      numErr = 0;
    }

    network = forward(network, item.input);
    char c =outputToChar(network.outputs, data.list);
  //  printf("%c -> %c\n",item.character, c);
    if(item.character != c)
      numErr++;
    numTot++;
    pre = item.character;
  }
  printf("%c : %f‰ \n", pre, 100.0 - ((numErr / numTot) * 100.0) );
  totReussite = totReussite + 100.0 - ((numErr / numTot) * 100.0);

  printf("total : %f\n", totReussite / 26.0);

  if(totReussite / 26.0 > network.maxAccuracy)
  {
    network.maxAccuracy = totReussite / 26.0;
    save_network(network, "bestnetwork.txt");
  }
  network.accuracy = totReussite / 26.0;

  return network;
}

struct Network forward(struct Network network, double* inputs)
{
//  printbinimage(inputs);


  /* hOutputs */

  for(size_t i = 0; i<network.numHidden;i++){
    network.hOutputs[i] = 0.0;
    for(size_t j = 0; j<network.numInput;j++){
      network.hOutputs[i] += inputs[j] * network.ihWeights[j][i];
    }
    network.hOutputs[i] += network.hBiases[i];
    network.hOutputsA[i] = network.hOutputsA[i];
    network.hOutputs[i] = sigmoid(network.hOutputs[i]);
  }
/*
  for(size_t i = 0; i<network.numHidden;i++)
  {
    printf("hOutputs[%zu] = %f\n", i,network.hOutputs[i]);
  }
  */

  /* outputs */

  for(size_t i = 0; i<network.numOutput;i++){

    network.outputs[i] = 0.0;
    for(size_t j = 0; j<network.numHidden;j++){
      network.outputs[i] += network.hOutputs[j] * network.hoWeights[j][i];
    }
    network.outputs[i] += network.oBiases[i];
    network.outputsA[i] = network.outputs[i];
    network.outputs[i] = sigmoid(network.outputs[i]);
  }

//  network.outputs = softmax(network.outputs, network.numOutput);
/*
  for(size_t i = 0; i<network.numOutput;i++)
  {
    printf("outputs[%zu] = %f\n", i,network.outputs[i]);
  }
  */

  return network;
}

struct Network new_Network(
    size_t numInput,
    size_t numHidden,
    size_t numOutput){

  struct Network network;
  network.numInput = numInput;
  network.numHidden = numHidden;
  network.numOutput = numOutput;
  network.accuracy = 0.0;
  network.maxAccuracy = 0.0;
  network.inputs = malloc(sizeof(double) * numInput);
  network.hBiases = malloc(sizeof(double) * numHidden);
  network.hOutputs = malloc(sizeof(double) * numHidden);
  network.hOutputsA = malloc(sizeof(double) * numHidden);
  network.oBiases = malloc(sizeof(double) * numOutput);
  network.outputs = malloc(sizeof(double) * numOutput);
  network.outputsA = malloc(sizeof(double) * numOutput);
  network.ihWeights = initMatrix(numInput, numHidden, 0.0);
  network.hoWeights = initMatrix(numHidden, numOutput, 0.0);
  printf("%s\n","network created");
  return network;
}

//charge le reseau de neurones a partir d'un fichier
struct Network load_network_from_file(char* path)
{
      FILE * fp;
      char * line = NULL;
      size_t len = strlen(path);
      ssize_t read;
      struct Network network;

      char * tabname = malloc(sizeof(char) * 5);
      *(tabname + 4) = '\0';
      size_t index1;
      size_t index2;
      float value;

      char * beginf;

      fp = fopen(path, "r");
      if (fp == NULL)
          exit(EXIT_FAILURE);

      char *c = malloc(sizeof(char) * 2);
      size_t numInput;
      size_t numHidden;
      size_t numOutput;
      read = getline(&line, &len, fp);
      numInput = atoi(line);
      read = getline(&line, &len, fp);
      numHidden = atoi(line);
      read = getline(&line, &len, fp);
      numOutput = atoi(line);
      network = new_Network(numInput, numHidden, numOutput);
      while ((read = getline(&line, &len, fp)) != -1)
      {
          for(size_t i = 0; i<4; i++)
            *(tabname + i) = *(line + i);

          if (strcmp(tabname, "hBia") == 0)
          {
            index1 = 0;
            size_t u = 8;
            while(*(line+u) != ']')
            {
              *c = *(line + u);
              index1 = index1 * 10 + atoi(c);
              u++;
            }

            beginf = line + u + 4;
            if(*beginf == '-'){
              value = atof(beginf+1) * -1.0f;}
            else{
              value = atof(beginf);}

            network.hBiases[index1] = value;
          }

          if (strcmp(tabname, "oBia") == 0)
          {
            index1 = 0;
            size_t u = 8;
            while(*(line+u) != ']')
            {
              *c = *(line + u);
              index1 = index1 * 10 + atoi(c);
              u++;
            }

            beginf = line + u + 4;
            if(*beginf == '-'){
              value = atof(beginf+1) * -1.0f;}
            else{
              value = atof(beginf);}

            network.oBiases[index1] = value;
          }

          if (strcmp(tabname, "ihWe") == 0)
          {
            index1 = 0;
            index2 = 0;
            size_t u = 10;
            while(*(line+u) != ']')
            {
              *c = *(line + u);
              index1 = index1 * 10 + atoi(c);
              u++;
            }
            u+=2;
            while(*(line+u) != ']')
            {
              *c = *(line+u);
              index2 = index2 * 10 + atoi(c);
              u++;
            }
            beginf = line + u + 4;
            if(*beginf == '-'){
              value = atof(beginf+1) * -1.0f;}
            else{
              value = atof(beginf);}

              network.ihWeights[index1][index2] = value;
          }

          if (strcmp(tabname, "hoWe") == 0)
          {
            index1 = 0;
            index2 = 0;
            size_t u = 10;
            while(*(line+u) != ']')
            {
              *c = *(line + u);
              index1 = index1 * 10 + atoi(c);
              u++;
            }
            u+=2;
            while(*(line+u) != ']')
            {
              *c = *(line+u);
              index2 = index2 * 10 + atoi(c);
              u++;
            }
            beginf = line + u + 4;
            if(*beginf == '-'){
              value = atof(beginf+1) * -1.0f;}
            else{
              value = atof(beginf);}

              network.hoWeights[index1][index2] = value;
          }
      }

      fclose(fp);
      if (line)
          free(line);
      free(tabname);
      free(c);
      printf("%s\n", "network loaded");
      return network;

}
