#include <iostream>
#include <vector>
#include <algorithm>
#include <thread>
#include <functional>

template<typename Tin, typename Tout>
Tout seqDC(Tin input,
        bool (*basecase)(Tin),
        Tout (*solve)(Tin),
        std::vector<Tin> (*divide)(Tin),
        Tout (*conquer)(std::vector<Tout>)) {

  if (basecase(input)) {
    return (solve(input));
  } else {
    auto subproblems = divide(input);

    std::transform(subproblems.begin(),
                   subproblems.end(),
                   subproblems.begin(),
                   [&](Tin x) {
                     auto res = seqDC(x, basecase, solve, divide, conquer);
                     return (res);
                   });

    auto result = conquer(subproblems);
    return (result);
  }
};

template<typename Tin, typename Tout>
Tout parDC(Tin input,
           bool (*basecase)(Tin),
           Tout (*solve)(Tin),
           std::vector<Tin> (*divide)(Tin),
           Tout (*conquer)(std::vector<Tout>),
           bool (*checkCutOff)(Tin)) {

  if (basecase(input)) {
    return (solve(input));
  } else {
    auto subproblems = divide(input);
    std::vector<Tout> results(subproblems.size());
    // list of threads
    std::vector<std::thread> threadList;

    if(!checkCutOff(subproblems[0])){
      std::cout<<"par"<<std::endl;
       auto threadLambda = [&results, basecase, solve, divide, conquer, checkCutOff, &subproblems](const int i) {
        results[i] = parDC(subproblems[i], basecase, solve, divide, conquer, checkCutOff);
      };

      for (auto i = 0; i != subproblems.size(); i++) {
        threadList.push_back(std::thread(threadLambda, i));
      }
    }else {
       auto threadLambda = [&results, basecase, solve, divide, conquer, checkCutOff, &subproblems](const int i) {
        results[i] = seqDC(subproblems[i], basecase, solve, divide, conquer);
      };
      for (auto i = 0; i != subproblems.size(); i++) {
        threadList.push_back(std::thread(threadLambda, i));
      }
    }



    for (std::thread &t: threadList) {                        // await thread termination
      t.join();
    }

    auto result = conquer(subproblems);
    return (result);
  }
};
