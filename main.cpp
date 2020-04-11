#include <iostream>
#include <chrono>
int main(int argc, char *argv[]) {

  if (argc == 1) {
    std::cout << "Usage is " << argv[0]
              << " numberOfIteration seed N M msecD nw" << std::endl;
    return (-1);
  }

  int nStep = atoi(argv[1]);
  int seed = atoi(argv[2]);
  int nRow = atoi(argv[3]);
  int nCol = atoi(argv[4]);
  int nw = atoi(argv[5]);

  bool states[nRow][nCol]{};
  bool statesTmp[nRow][nCol]{};

  std::srand(seed);

  // Random initialization junping the border
  for (int row = 1; row < nRow - 1; ++row) {
    for (int col = 1; col < nCol - 1; ++col) {
      states[row][col] = std::rand() % 2;
    }
  }

  for (int row = 0; row < nRow; ++row) {
    for (int col = 0; col < nCol; ++col) {
      std::cout << statesTmp[row][col] << " ";
    }
    std::cout << std::endl;
  }

  int sum = 0;
  // Run nStep
  auto tstart = std::chrono::high_resolution_clock::now();
  for (int k = 0; k < nStep; ++k) {

    std::cout << k << std::endl;

    for (int row = 1; row < nRow - 1; ++row) {
      for (int col = 1; col < nCol - 1; ++col) {
        sum = states[row - 1][col - 1] +
            states[row - 1][col] +
            states[row - 1][col + 1] +
            states[row][col - 1] +
            states[row][col + 1] +
            states[row + 1][col - 1] +
            states[row + 1][col] +
            states[row + 1][col + 1];

        if (sum > 3 || sum < 2) {
          statesTmp[row][col] = false;
        } else {
          statesTmp[row][col] = true;
        }
      }
    }
    for (int row = 0; row < nRow; ++row) {
      for (int col = 0; col < nCol; ++col) {
        std::cout << statesTmp[row][col] << " ";
        states[row][col] = statesTmp[row][col];
      }
      std::cout << std::endl;
    }

  }
  auto elapsed = std::chrono::high_resolution_clock::now() - tstart;
  auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
  std::cout << "Spent " << msec << " msecs with " << nw << " threads " << std::endl;
}

