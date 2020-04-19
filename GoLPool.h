//
// Created by gs1010 on 18/04/20.
//
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include "taskQueue.h"

#ifndef LOF__GOLPOOL_H_
#define LOF__GOLPOOL_H_
class GoLPool {
 private:
  bool *states;
  bool *statesTmp;
  const int nStep;
  const int nCol;
  const int nRow;
  const int nWorker;
  std::vector<std::thread *> workers{};
  std::mutex m;
  std::condition_variable cv;
  std::atomic<int> counter = 0;
  taskQueue<int> taskq;
  bool on = true;

 public:
  ~GoLPool() {
    delete (states);
    delete (statesTmp);
    for (int i = 0; i < workers.size(); i++) {
      workers[i]->detach();
      delete (workers[i]);
    }

    workers.clear();

  }
  GoLPool(const int userStep, const int userRow, const int userCol, const int seed, const int nw)
      : nWorker(nw),
        nCol(userCol),
        nRow(userRow),
        states(new bool[userRow * userCol]{}),
        statesTmp(new bool[userRow * userCol]{}),
        nStep(userStep),
        workers(nw) {
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

    for (int iter = 0; iter < nStep; iter++) {
      counter = ((nRow - 2) * (nCol - 2));
      // trigger howmany runs
      for (int j = nCol + 1; j < nRow * (nCol - 1); j++) {
        if ((j % nCol) && j % nCol != nCol - 1) {
          taskq.push(j);
        }
      }

      std::unique_lock<std::mutex> lock(m);
      cv.wait(lock, [=]() { return counter == 0; });
      std::swap(states, statesTmp);
      //PrintStates();

    }

    for (int i = 0; i < nWorker; i++) {
      taskq.push(-1);
    }
  }

  void RunWithTime() {
    for (int i = 0; i < nWorker; i++)
      workers[i] = new std::thread(&GoLPool::RunParallel, this);

    auto tstart = std::chrono::high_resolution_clock::now();
    Run();
    auto elapsed = std::chrono::high_resolution_clock::now() - tstart;
    auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
    std::cout << "Spent " << msec << " msecs with " << std::endl;
  }

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
  void RunParallel(/*bool *states,
                          bool *passedStatesTmp,
                          taskQueue<int> &passedTaskq,
                          std::mutex &passedM,
                          std::atomic<int> &passedCounter,
                          std::condition_variable &passedCv,
                          const int passedNCol*/
  ) {
    int sumNeighbours;
    while (true) {
      auto index = taskq.pop();
      if (index == -1)
        return;

      //std::cout << index << std::endl;
      sumNeighbours = states[index - nCol - 1] +
          states[index - nCol] +
          states[index - nCol + 1] +
          states[index - 1] +
          states[index + 1] +
          states[index + nCol - 1] +
          states[index + nCol] +
          states[index + nCol + 1];

      if (sumNeighbours == 3) {
        statesTmp[index] = true;
      } else statesTmp[index] = sumNeighbours == 2 && states[index];

      std::lock_guard<std::mutex> lk(m);
      counter--;
      cv.notify_all();
    }
  }

#pragma clang diagnostic pop
};

#endif //LOF__GOLPOOL_H_
