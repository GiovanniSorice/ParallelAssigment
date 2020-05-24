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

  simpleImageProcessing SIP(imgPath, repetition);
  SIP.sequential();
  SIP.openMPImpl(nw);
  SIP.par(nw);

  return 0;
}