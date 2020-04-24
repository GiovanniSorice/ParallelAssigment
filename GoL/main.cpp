#include <iostream>
#include <chrono>
#include <omp.h>
#include "GoLSeq.h"
#include "GoLThread.h"
#include "GoLPool.h"
#include "GoLOpenMP.h"

int main(int argc, char *argv[]) {

  if (argc == 1) {
    std::cout << "Usage is " << argv[0]
              << " numberOfIteration seed N M nw" << std::endl;
    return (-1);
  }

  int nStep = atoi(argv[1]);
  int seed = atoi(argv[2]);
  int const nRow = atoi(argv[3]);
  int const nCol = atoi(argv[4]);
  int nw = atoi(argv[5]);

  /**
   Sequential run
   */

  GoLSeq seq(nStep, nRow, nCol, seed);
  seq.PrintStates();
  seq.RunWithTime();


  /**
    Parallel fork-join implementation run
    */

  GoLThread parT(nStep, nRow, nCol, seed, nw);
  parT.PrintStates();
  parT.RunWithTime();


  /**
    Parallel OpenMP implementation run
    */

  GoLOpenMP parOMP(nStep, nRow, nCol, seed, nw);
  parOMP.PrintStates();
  parOMP.RunWithTime();


  /**
  Parallel Pool of standard thread implementation run
  */

  GoLPool parPool(nStep, nRow, nCol, seed, nw);
  parPool.PrintStates();
  parPool.RunWithTime();

}

