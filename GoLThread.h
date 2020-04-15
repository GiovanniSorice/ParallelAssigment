//
// Created by gs1010 on 15/04/20.
//
#ifndef LOF__GOLTHREAD_H_
#define LOF__GOLTHREAD_H_
#include <thread>
#include <deque>

class GoLThread {

 private:
  bool *states;
  bool *statesTmp;
  const int nStep;
  const int nCol;
  const int nRow;
  const int nWorker = 8;

 public:
  ~GoLThread() {
    delete (states);
    delete (statesTmp);
  }
  GoLThread(const int userStep, const int userRow, const int userCol, const int seed)
      : nCol(userCol),
        nRow(userRow),
        states(new bool[userRow * userCol]{}),
        statesTmp(new bool[userRow * userCol]{}),
        nStep(userStep) {
    std::srand(seed);
    // Random initialization junping the border
    for (int row = 1; row < nRow - 1; ++row) {
      for (int col = 1; col < nCol - 1; ++col) {
        states[nCol * row + col] = std::rand() % 2;
      }
    }
  }
  void PrintStates() {
    for (int row = 0; row < nRow; ++row) {
      for (int col = 0; col < nCol; ++col) {
        std::cout << states[row * nCol + col] << " ";
      }
      std::cout << std::endl;
    }
    std::cout << std::endl;
  }

  void Run() {
    std::deque<std::thread *> coda;
    //TODO: Come spartisco il lavoro?
    for (int k = 0; k < nStep; ++k) {
      // create threads
      for (int i = 0; i < nWorker; ++i) {
        coda.push_back(new std::thread(RunParallel, states, statesTmp, nCol, 1, 100, 1, 100));
      }
      for (auto it = coda.begin(); it != coda.end(); ++it) {
        (*it)->join();
      }

      while (!coda.empty()) {
        delete (*coda.end());
        coda.pop_back();
      }



      //PrintStates();
      std::swap(states, statesTmp);
    }
  }

  void RunWithTime() {
    auto tstart = std::chrono::high_resolution_clock::now();
    Run();
    auto elapsed = std::chrono::high_resolution_clock::now() - tstart;
    auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
    std::cout << "Spent " << msec << " msecs with " << std::endl;
  }

  static void RunParallel(bool *passedStates,
                          bool *passedStatesTmp,
                          const int passedNCol,
                          const int startRow,
                          const int endRow,
                          const int startCol,
                          const int endCol) {
    int sum;
    std::cout << startRow << std::endl;
    for (int row = startRow; row < endRow - 1; ++row) {
      for (int col = startCol; col < endCol - 1; ++col) {
        sum = passedStates[(row - 1) * passedNCol + col - 1] +
            passedStates[(row - 1) * passedNCol + col] +
            passedStates[(row - 1) * passedNCol + col + 1] +
            passedStates[row * passedNCol + col - 1] +
            passedStates[row * passedNCol + col + 1] +
            passedStates[(row + 1) * passedNCol + col - 1] +
            passedStates[(row + 1) * passedNCol + col] +
            passedStates[(row + 1) * passedNCol + col + 1];

        if (sum > 3 || sum < 2) {
          passedStatesTmp[row * passedNCol + col] = false;
        } else {
          passedStatesTmp[row * passedNCol + col] = true;
        }
      }

    }

  }
};

#endif //LOF__GOLTHREAD_H_
