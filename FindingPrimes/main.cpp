#include "master_worker.h"
#include "parallel_for_exec.h"

int main(int argc, char *argv[]) {
  if (argc < 3) {
    std::cerr << "use: " << argv[0] << "nworker number1 number2\n";
    return -1;
  }
  const size_t nworkers = std::stol(argv[1]);

  unsigned long long n1 = std::stoll(argv[2]);
  unsigned long long n2 = std::stoll(argv[3]);
  bool print_primes = false;

  //if (argc >= 4)  print_primes = (std::string(argv[4]) == "on");


  master_worker_primes msp(n1, n2, nworkers);
  msp.run();

  master_worker_primes pf(n1, n2, nworkers);
  pf.run();

}
