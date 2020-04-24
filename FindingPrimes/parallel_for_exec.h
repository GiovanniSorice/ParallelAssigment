//
// Created by gs1010 on 23/04/20.
//

#ifndef FINDINGPRIMES__PARALLEL_FOR_EXEC_H_
#define FINDINGPRIMES__PARALLEL_FOR_EXEC_H_

#include <cstddef>
#include <ff/parallel_for.hpp>
#include <mutex>
class parallel_for_exec {
 private:
  const unsigned long long start;
  const unsigned long long end;
  const std::size_t nworkers;

 public:
  parallel_for_exec(const unsigned long long start, const unsigned long long end, const std::size_t nworkers)
      : nworkers(nworkers), start(start), end(end) {}

  static bool is_prime(unsigned long long n) {
    if (n <= 3) return n > 1; // 1 is not prime !

    if (n % 2 == 0 || n % 3 == 0) return false;

    for (unsigned long long i = 5; i * i <= n; i += 6) {
      if (n % i == 0 || n % (i + 2) == 0)
        return false;
    }
    return true;
  }

  void run() {

    std::vector<unsigned long long> prime_numbers;
    prime_numbers.reserve((size_t) (end - start) / log(start));

    ff::ParallelFor pf(nworkers);
    std::mutex vector_mutex;

    ff::ffTime(ff::START_TIME);

    pf.parallel_for(start, end,    // start, stop indexes
                        1,          // step size
                        1,          // chunk size (0=static, >0=dynamic)
                        [&](unsigned long long n) {
                          if (is_prime(n)) {
                            vector_mutex.lock();
                            prime_numbers.push_back(n);
                            vector_mutex.unlock();
                          }
                        });
    ff::ffTime(ff::STOP_TIME);
    /*
    for (auto &&prime: prime_numbers) {
      std::cout << prime << ", ";
    }
     */
    std::cout << "Primes found: " << prime_numbers.size() << std::endl;
    std::cout << "nworkers: " << nworkers << " time: " << ff::ffTime(ff::GET_TIME) << "\n";

  };
};

#endif //FINDINGPRIMES__PARALLEL_FOR_EXEC_H_
