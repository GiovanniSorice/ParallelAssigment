# Simple Image Processing with OpenCV
The purpose of this assignment was to implement a parallel version of the application over images of OpenCV filters. I started from a sequential implementation, the sequential methods in [this file](simpleImageProcessing.h).

## Challenges
A problem that I faced since the beginning was that each filter takes different times to be applied. 
For this reason, I think that the best implementation could be the one in which every filter have a pool of worker associated.
Also, I think that the number of worker of each pool has to be directly proportional to the time spent applying the filter.

## Building
To generate the build system do:  
```
g++ -std=c++17 -O3 -I /path/to/opencv4/include/opencv4 -L /path/to/opencv4/lib main.cpp -o simpleImageProcessing -lopencv_core -lopencv_imgproc -lopencv_imgcodecs -fopenmp -pthread

```

## Running
Just run it with:

```./simpleImageProcessing path/to/images nWorker nRepetition for each image``` 

Where the nWorker is the total number of worker(more precisely thread) that we would use.
Where the nRepetition is the number of times that we would load each image(use it if you have a small dataset and would check the performance with bigger dataset).


## Acknowledgments
This project was developed for an assignment of the course [Parallel and Distributed Systems](http://didawiki.di.unipi.it/doku.php/magistraleinformaticanetworking/spm/sdpm09support) at University of Pisa under the guide of [Prof. Marco Danelutto](http://calvados.di.unipi.it/paragroup/danelutto/) and [Prof. Massimo Torquati](http://calvados.di.unipi.it/paragroup/torquati/).
