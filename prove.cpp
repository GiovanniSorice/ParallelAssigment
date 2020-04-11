#include <iostream>

struct state {
  bool now;
  bool next = false;
};
int main() {

  int nRow = 10;
  int nCol = 10;
  int nStep = 10;

  state *states = new state[nRow * nCol];

  std::srand(20);
  int randomval = std::rand() % 2;

  // Random initialization
  for (int i = 0; i < nRow * nCol; i++) {
    states[i].now = std::rand() % 2;
  }

  // Setting borders to false
  for (int i = 0; i < nCol; i++) {
    states[i].now = false;
    states[(nRow - 1) * nCol + i].now = false;
  }

  for (int i = 0; i < nRow; i++) {
    states[nCol * i].now = false;
    states[nRow * (i + 1) - 1].now = false;
  }
/*
  for (int k = 0; k < nRow; ++k) {
    for (int i = 0; i < nCol; ++i) {
      std::cout << states[nRow * k + i].now << " ";
    }
    std::cout << std::endl;
  }
*/

  // Run nStep
  for (int k = 0; k < nStep; ++k) {
    for (int row = 1; row < nRow - 1; ++row) {
      for (int col = 1; col < nCol - 1; ++col) {
        if (states[nRow * row + col].now)
      }
    }
  }
}