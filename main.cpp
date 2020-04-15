#include <iostream>
#include <chrono>
#include <omp.h>
#include "GoLSeq.h"
#include "GoLThread.h"

int main(int argc, char *argv[]) {

  if (argc == 1) {
    std::cout << "Usage is " << argv[0]
              << " numberOfIteration seed N M msecD nw" << std::endl;
    return (-1);
  }

  int nStep = atoi(argv[1]);
  int seed = atoi(argv[2]);
  int const nRow = atoi(argv[3]);
  int const nCol = atoi(argv[4]);
  int nw = atoi(argv[5]);

  GoLSeq seq(nStep, nRow, nCol, seed);
  seq.PrintStates();
  /*
  auto tstart = std::chrono::high_resolution_clock::now();
  seq.Run();
  auto elapsed = std::chrono::high_resolution_clock::now() - tstart;
  auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
  std::cout << "Spent " << msec << " msecs with " << nw << " threads " << std::endl;
   */
  seq.RunWithTime();

}

