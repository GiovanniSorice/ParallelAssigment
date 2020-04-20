//
// Created by gs1010 on 18/04/20.
//
#import <mutex>
#import <condition_variable>
#import <deque>

#ifndef LOF__TASKQUEUE_H_
#define LOF__TASKQUEUE_H_

template<typename T>
class taskQueue {
 private:
  std::mutex d_mutex;
  std::condition_variable d_condition;
  std::deque<T> d_queue;

 public:
  taskQueue() {}
  taskQueue(const int n) : d_queue(n) {}
  void push(T const &value) {
    {
      std::unique_lock<std::mutex> lock(d_mutex);
      d_queue.push_front(value);
    }
    d_condition.notify_one();
  }

  T pop() {
    std::unique_lock<std::mutex> lock(this->d_mutex);
    this->d_condition.wait(lock, [=] { return !this->d_queue.empty(); });
    T rc(std::move(d_queue.back()));
    d_queue.pop_back();
    return rc;
  }
};

#endif //LOF__TASKQUEUE_H_
