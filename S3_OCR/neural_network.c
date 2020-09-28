#include "neural_network.h"
/*
int main() {

  printf("%s\n", "hello world");
  struct Network nw;

  if(1){ //mettre a 0 si on veut charger depuis un fichier
    nw = new_Network(1024,200,26);
    nw = init_Network(nw);
  }else
  {
    nw = load_network_from_file("savefile.txt");
  }
  printf("%s\n","network created" );
  const char* trainDataPath = "Train_Session_1/";
  const char* charlist = "abcdefghijklmnopqrstuvwxyz";
  struct trainData td = loadDataFromFolder(trainDataPath, charlist);
  printf("%s\n","data loaded" );


  nw = train(nw, td, 1000, 0.1, 0.01);




  freeTrainData(td);
  save_network(nw, "savefile.txt");
  freeNetwork(nw);
  return 0;
}
*/


struct Network init_Network(struct Network network)
{
  srand(time(NULL));

  /* init hBiases */
  for(size_t i = 0;i<network.numHidden;i++)
    network.hBiases[i] = getRandom(network.numInput);

  /* init ihWeights */
  for(size_t i = 0;i<network.numInput;i++)
    for(size_t j = 0;j<network.numHidden;j++)
    {
      network.ihWeights[i][j] = getRandom(network.numInput);

    }


  /* init oBiases */
  for(size_t i = 0;i<network.numOutput;i++)
    network.oBiases[i] = getRandom(network.numInput);

  /* init hoWeights */
  for(size_t i = 0;i<network.numHidden;i++)
    for(size_t j = 0;j<network.numOutput;j++)
      network.hoWeights[i][j] = getRandom(network.numInput);

  return network;
}





struct Network backward(
              struct Network network,
              double* tValues,
              double learnRate,
              double momentum){

  double** hoGrads = initMatrix(network.numHidden,network.numOutput, 0.0);
  double* obGrads = malloc(sizeof(double) * network.numOutput);
  double** ihGrads = initMatrix(network.numInput, network.numHidden, 0.0);
  double* hbGrads = malloc(sizeof(double) * network.numHidden);

  double** ihPrevWeightsDelta = initMatrix(network.numInput, network.numHidden, 0.0);
  double* hPrevBiasesDelta = malloc(sizeof(double) * network.numHidden);
  for(size_t i = 0; i<network.numHidden;i++)
    hPrevBiasesDelta[i] = 0.0;

  double** hoPrevWeightsDelta = initMatrix(network.numHidden, network.numOutput, 0.0);
  double* oPrevBiasesDelta = malloc(sizeof(double) * network.numOutput);
  for(size_t i = 0; i<network.numOutput;i++)
    oPrevBiasesDelta[i] = 0.0;

  double* oSignals = malloc(sizeof(double) * network.numOutput);
  double* hSignals = malloc(sizeof(double) * network.numHidden);

  //double derivative = 0.0;
  //double errorSignal = 0.0;
  momentum = 0 * momentum;

  double* oDeltas = malloc(sizeof(double) * network.numOutput);
  double* hDeltas = malloc(sizeof(double) * network.numHidden);


  // 1. Compute output node signals with softmax
  /*
  for(size_t k = 0;k<network.numOutput;++k)
  {
    errorSignal = tValues[k] - network.outputsA[k];
    derivative = (1 - network.outputs[k]) * network.outputs[k]; //softmax
    oSignals[k] = errorSignal * derivative;
  }
  */

  // COMPUTE oDelta
  for(size_t k = 0; k<network.numOutput; k++)
    oDeltas[k] = sigmoid_prime(network.outputsA[k]) * (tValues[k] - network.outputs[k]);

  // UPDATE Hidden-to-Output weights
  for(size_t j = 0; j<network.numHidden;++j)
    for(size_t k = 0;k<network.numOutput;++k)
      network.hoWeights[j][k] += network.hOutputs[j] * oDeltas[k];

  // UPDATE output biases
  for(size_t k = 0; k<network.numOutput; k++)
    network.oBiases[k] += oDeltas[k] * learnRate;

  network = forward(network, network.inputs);

  //compute hDeltas
  for(size_t k = 0; k<network.numHidden; k++)
  {
    double sum = 0.0;
    for(size_t j = 0; j<network.numOutput;++j)
      sum += oDeltas[j] * network.hoWeights[k][j];
    hDeltas[k] = sigmoid_prime(network.hOutputsA[k]) * sum;
  }

  // UPDATE input-to-Hidden weights
  for(size_t j = 0; j<network.numInput;++j)
    for(size_t k = 0;k<network.numHidden;++k)
      network.ihWeights[j][k] += network.inputs[j] * hDeltas[k];

  // UPDATE hidden biases
  for(size_t k = 0; k<network.numHidden; k++)
    network.hBiases[k] += hDeltas[k] * learnRate;



  // 2. Compute Hidden-to-Output weight gradients using output signals
/*
  for(size_t j = 0; j<network.numHidden;++j)
    for(size_t k = 0;k<network.numOutput;++k)
      hoGrads[j][k] = oSignals[k] * network.hOutputs[j];

  // 2b. Compute output bias gradients using output signals
  for(size_t k = 0; k<network.numOutput;++k)
    obGrads[k] = oSignals[k] * 1.0;
*/
  // 3. Compute hidden node signals
//  for(size_t j = 0;j<network.numHidden;++j)
//  {
  //  derivative = (1 + network.hOutputsA[j]) * (1 - network.hOutputsA[j]); //for tanh (moi jveux sigmoid)
    /*derivative = sigmoid(network.hOutputsA[j]) * ( 1 - sigmoid(network.hOutputsA[j])); //for sigmoide
    double sum = 0.0;
    for(size_t k = 0; k < network.numOutput;++k)
      sum+= oSignals[k] * network.hoWeights[j][k];
    hSignals[j] = derivative * sum;*/
//    errorSignal = sigmoid_prime(network.hOutputsA[j]) *
//  }
/*
  // 4. Compute input-hidden weights gradients
  for(size_t i = 0; i< network.numInput;++i)
    for(size_t j = 0; j<network.numHidden;++j)
      ihGrads[i][j] = hSignals[j] * network.inputs[i];

  //4b. Compute hiddden node bias gradients
  for(size_t j = 0; j<network.numHidden;++j)
    hbGrads[j] = hSignals[j] * 1.0;
*/
  /* update weights and biases */
/*
  //update input-to-hidden weights
  for(size_t i =0; i<network.numInput;++i)
  {
    for(size_t j = 0;j<network.numHidden;++j)
    {
      double delta = ihGrads[i][j] * learnRate;
      network.ihWeights[i][j] += delta;
      network.ihWeights[i][j] += ihPrevWeightsDelta[i][j] * momentum;
      ihPrevWeightsDelta[i][j] = delta;
    }
  }

  //update hidden biases
  for(size_t j = 0;j<network.numHidden;++j)
  {
    double delta = hbGrads[j] * learnRate;
    network.hBiases[j] += delta;
    network.hBiases[j] += hPrevBiasesDelta[j] * momentum;
    hPrevBiasesDelta[j] = delta;
  }

  //update Hidden-to-Output weights
  for(size_t i =0; i<network.numHidden;++i)
  {
    for(size_t j = 0;j<network.numOutput;++j)
    {
      double delta = hoGrads[i][j] * learnRate;
      network.hoWeights[i][j] += delta;
      network.hoWeights[i][j] += hoPrevWeightsDelta[i][j] * momentum;
      hoPrevWeightsDelta[i][j] = delta;
    }
  }

  //update hidden biases
  for(size_t j = 0;j<network.numOutput;++j)
  {
    double delta = obGrads[j] * learnRate;
    network.oBiases[j] += delta;
    network.oBiases[j] += oPrevBiasesDelta[j] * momentum;
    oPrevBiasesDelta[j] = delta;
  }
*/
  freeMatrix(hoPrevWeightsDelta, network.numHidden);
  freeMatrix(ihPrevWeightsDelta, network.numInput);
  freeMatrix(hoGrads, network.numHidden);
  freeMatrix(ihGrads, network.numInput);
  free(hPrevBiasesDelta);
  free(oPrevBiasesDelta);
  free(obGrads);
  free(hbGrads);
  free(oSignals);
  free(hSignals);
  free(oDeltas);
  free(hDeltas);

  return network;

}




struct Network train(
              struct Network network,
              struct trainData traindata,
              int maxEpochs,
              double learnRate,
              double momentum){

  double *xValues;
  double *tValues;

  network = test_network(network, traindata);

  network = forward(network, traindata.items[0].input);
  network = test_network(network, traindata);
/*
  for(size_t i = 0; i<network.numOutput;i++)
  {
    printf("outputs[%zu] = %f\n", i,network.outputs[i]);
  }
  */

  for(int epoch = 0; epoch < maxEpochs; epoch++)
  {
    printf("epoch %d :\n", epoch );
    traindata = shuffleData(traindata);
    traindata.currentIndex++;
    struct trainItem* trainitem = getNextData(traindata);
    while(trainitem != NULL)
    {
    //  printf("%c\n", trainitem->character);

      xValues = trainitem->input;
      tValues = trainitem->target;
      network = forward(network, xValues);
      network = backward(network, tValues, learnRate, momentum);
      traindata.currentIndex++;
      trainitem = getNextData(traindata);

    }
    network = test_network(network, traindata);

  }

  return network;
}
