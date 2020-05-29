# File compressor with Miniz
The purpose of this assignment was to implement a parallel version of fast file compressor with Miniz. I started from a given sequential implementation, the one in [this file](ffc_pipe.cpp).

## Building
To generate the build system do:  
```
make

```

## Running
Just run it with:

```./ffc_farm nWorker path/to/file-or-directory ``` 

Where the nWorker is the total number of worker that you would use in the farm.

## Acknowledgments
This project was developed for an assignment of the course [Parallel and Distributed Systems](http://didawiki.di.unipi.it/doku.php/magistraleinformaticanetworking/spm/sdpm09support) at University of Pisa under the guide of [Prof. Marco Danelutto](http://calvados.di.unipi.it/paragroup/danelutto/) and [Prof. Massimo Torquati](http://calvados.di.unipi.it/paragroup/torquati/).
