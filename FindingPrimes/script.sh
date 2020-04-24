#!/bin/sh
clear
echo "Running first test."

echo "nw time" >>resultsMW.txt
for row in 1000000 10000000 40000000; do
  echo row $row
  for w in {1..250}; do
    echo $w
    cmake-build-release-xionphi/FindingPrimes $w 0 ${row} >>resultsPF${row}M.txt
  done
done
echo "Fine."
