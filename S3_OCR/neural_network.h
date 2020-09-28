#include <stdio.h>
#include <stdlib.h>
#include "network_helper.h"


struct Network init_Network(struct Network network);
struct Network backward(
              struct Network network,
              double* inputs,
              double learnRate,
              double momentum
            );

struct Network train(
              struct Network network,
              struct trainData traindata,
              int maxEpochs,
              double learnRate,
              double momentum
            );
