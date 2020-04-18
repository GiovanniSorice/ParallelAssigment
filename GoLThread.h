//
// Created by gs1010 on 15/04/20.
//
#ifndef LOF__GOLTHREAD_H_
#define LOF__GOLTHREAD_H_
#include <thread>
#include <deque>
#include <cmath>

class GoLThread {

 private:
  bool *states;
  bool *statesTmp;
  const int nStep;
  const int nCol;
  const int nRow;
  const int nWorker;

 public:
  ~GoLThread() {
    delete (states);
    delete (statesTmp);
  }
  GoLThread(const int userStep, const int userRow, const int userCol, const int seed, const int nw)
      : nWorker(nw),
        nCol(userCol),
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
    std::deque < std::thread * > coda;

    int splitElem = std::floor((float) (nCol) * (float) (nRow - 2) / (float) nWorker);
    int remaining;
    int index;
    for (int k = 0; k < nStep; ++k) {
      // create threads
      index = nCol;
      remaining = (nCol) * (nRow - 2) % nWorker;
      for (int i = 0; i < nWorker; ++i) {

        coda.emplace_back(new std::thread(RunParallel,
                                          states,
                                          statesTmp,
                                          nCol,
                                          index,
                                          splitElem + (remaining ? 1 : 0)));
        index += splitElem + (remaining ? 1 : 0);

        if (remaining)
          remaining--;

        //Da aggiungere possibilimente un controllo che non crea thread in eccesso
      }

      for (auto &it : coda) {
        if (it->joinable())
          it->join();
      }

      while (!coda.empty()) {
        delete (*coda.end());
        coda.pop_back();
      }

      std::swap(states, statesTmp);
      PrintStates();

    }
  }

  void RunWithTime() {
    auto tstart = std::chrono::high_resolution_clock::now();
    Run();
    auto elapsed = std::chrono::high_resolution_clock::now() - tstart;
    auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
    //std::cout << "Spent " << msec << " msecs with " << std::endl;
  }

  static void RunParallel(bool *passedStates,
                          bool *passedStatesTmp,
                          const int passedNCol,
                          const int index,
                          const int stepElem
  ) {

    int sum;
    //std::cout << index << " " << stepElem << " " << std::endl;

    for (int i = index; i < index + stepElem; ++i) {
      if (!(i % passedNCol) || (i % passedNCol) == (passedNCol - 1))
        continue;

      //std::cout << i << " " << std::endl;

      sum = passedStates[i - passedNCol - 1] +
          passedStates[i - passedNCol] +
          passedStates[i - passedNCol + 1] +
          passedStates[i - 1] +
          passedStates[i + 1] +
          passedStates[i + passedNCol - 1] +
          passedStates[i + passedNCol] +
          passedStates[i + passedNCol + 1];

      //std::cout << i << " " << sum << " " << std::endl;

      if (sum == 3) {
        passedStatesTmp[i] = true;
      } else passedStatesTmp[i] = sum == 2 && passedStates[i];
    }


/*
    for (int row = startRow; row < endRow; ++row) {
      for (int col = startCol; col < endCol; ++col) {
        sum = passedStates[(row - 1) * passedNCol + col - 1] +
            passedStates[(row - 1) * passedNCol + col] +
            passedStates[(row - 1) * passedNCol + col + 1] +
            passedStates[row * passedNCol + col - 1] +
            passedStates[row * passedNCol + col + 1] +
            passedStates[(row + 1) * passedNCol + col - 1] +
            passedStates[(row + 1) * passedNCol + col] +
            passedStates[(row + 1) * passedNCol + col + 1];

        if (sum == 3) {
          passedStatesTmp[row * passedNCol + col] = true;
        } else passedStatesTmp[row * passedNCol + col] = sum == 2 && passedStates[row * passedNCol + col];
      }
    }
    */
  }
};

#endif //LOF__GOLTHREAD_H_
