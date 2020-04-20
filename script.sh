#!/bin/sh
clear
echo "Running first test."

for row in 10000; do
  echo "nw time col row" >>resultsOMP${row}.txt
  echo row $row
  for w in {1..250}; do
    echo $w
    cmake-build-release-xionphi/GoL 15 30 ${row} ${row} $w >>resultsOMP${row}.txt
  done
done
echo "Fine."
