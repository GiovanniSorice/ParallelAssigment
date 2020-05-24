#include <iostream>

// keep in mind that all or some of these includes
// may change depending on the library installation and version
#include <opencv2/core.hpp>
#include "opencv2/imgproc.hpp"
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <string>
#include <filesystem>
#include <vector>
#include <functional>
#include <chrono>
#include "simpleImageProcessing.h"
int main(int argc, char *argv[]) {

  if (argc != 4) {
    std::cout << "Usage is: " << argv[0] << " imgPath nw repetition\n";
    return (-1);
  }

  std::string imgPath = argv[1];
  long nw = atoi(argv[2]);
  long repetition = atoi(argv[3]);

  simpleImageProcessing a(imgPath, repetition);
  //a.sequential();
  //a.openMPImpl(nw);

  std::cout << "nw time(ms)" << std::endl;
  for (int i = 1; i < 257; i++) {
    a.par(i);
  }
  return 0;
}