//
// Created by gs1010 on 22/04/20.
//

#include <ff/pipeline.hpp>
#include <ff/farm.hpp>
#include <iostream>
#include <vector>
#include <tuple>
#include <cmath>



#ifndef FINDINGPRIMES__MASTER_WORKER_H_
#define FINDINGPRIMES__MASTER_WORKER_H_

struct firstThirdStage : ff::ff_node_t<std::vector<unsigned long long>> {
  bool is_prime(unsigned long long n) {
    if (n <= 3) return n > 1; // 1 is not prime !

    if (n % 2 == 0 || n % 3 == 0) return false;

    for (unsigned long long i = 5; i * i <= n; i += 6) {
      if (n % i == 0 || n % (i + 2) == 0)
        return false;
    }
    return true;
  }

  firstThirdStage(const size_t nworker,unsigned long long start, unsigned long long end):nworker(nworker),n1(start),n2(end){
    results.reserve((size_t)(n2-n1)/log(n1));
  }

  std::vector<unsigned long long>* svc(std::vector<unsigned long long> *task) override {
    int work_adj = 2;
    if (task == nullptr) {
      const unsigned long long step = std::floor((float) (n2 - n1 + 1) / (float) (nworker * work_adj));
      const unsigned long long remaining = (n2 - n1 + 1) % (nworker * work_adj);
      unsigned long long current_position = 0;
      for (size_t i = 0; i < nworker * work_adj; ++i) {
        ff_send_out(new std::pair<unsigned long long, unsigned long long>(current_position, (i + 1) * step));
        current_position += step + 1;
      }

      if(remaining){
        unsigned long long prime = nworker * work_adj * step;

        while( (prime++) <= n2)
          if (is_prime(prime)) results.push_back(prime);
      }

      return GO_ON;
    }
    std::vector<unsigned long long> &t = *task;

    results.insert( results.end(), t.begin(), t.end() );

    delete task;
    if (++ntasks == nworker * work_adj) return EOS;
    return  GO_ON;
  };

  void svc_end() override {
    std::sort(results.begin(), results.end());
    std::cout << " Primes found: " << results.size() << std::endl;
  };

  const size_t nworker;
  size_t ntasks=0;
  const unsigned long long n1;
  const unsigned long long n2;
  std::vector<unsigned long long> results;
};

struct secondStage : ff::ff_node_t<std::pair<unsigned long long, unsigned long long>, std::vector<unsigned long long>> {

  static bool is_prime(unsigned long long n) {
    if (n <= 3) return n > 1; // 1 is not prime !

    if (n % 2 == 0 || n % 3 == 0) return false;

    for (unsigned long long i = 5; i * i <= n; i += 6) {
      if (n % i == 0 || n % (i + 2) == 0)
        return false;
    }
    return true;
  }

  std::vector<unsigned long long> *svc(std::pair<unsigned long long, unsigned long long> *task) override {
    std::pair<unsigned long long, unsigned long long> &t = *task;

    auto *results = new std::vector<unsigned long long>();

    auto prime = std::get<0>(t);

    while ((prime++) <= std::get<1>(t)) {
      if (is_prime(prime)) results->push_back(prime);
    }

    delete task;
    return results;
  };
  // see http://en.wikipedia.org/wiki/Primality_test
};
  class master_worker_primes {
   private:
    unsigned long long start;
    unsigned long long end;
    size_t nworkers;

   public:
    master_worker_primes(unsigned long long n1, unsigned long long n2, unsigned long long nworkers) : start(n1),
                                                                                                      end(n2), nworkers(nworkers) {
    }

    int run() {

      firstThirdStage firstthird(nworkers, start, end);

      std::vector<std::unique_ptr<ff::ff_node> > W;
      for (size_t i = 0; i < nworkers; ++i) W.push_back(ff::make_unique<secondStage>());

      ff::ff_Farm<std::vector<unsigned long long>> farm(std::move(W), firstthird);
      farm.remove_collector(); // needed because the collector is present by default in the ff_Farm
      farm.wrap_around();   // this call creates feedbacks from Workers to the Emitter
      //farm.set_scheduling_ondemand(); // optional

      ff::ffTime(ff::START_TIME);
      if (farm.run_and_wait_end() < 0) {
        ff::error("running farm");
        return -1;
      }
      ff::ffTime(ff::STOP_TIME);
      std::cout << "nworkers: "<<nworkers << " time: "<< ff::ffTime(ff::GET_TIME)<<"\n";
      return 0;
    }

};

#endif //FINDINGPRIMES__MASTER_WORKER_H_
