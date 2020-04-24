In the folder "code" you can find the code in which there are 3 files:
main.cpp: the file that you can use for do some tests;
master_worker.h: in which there is the code of master-worker implementation;
parallel_for_exec.h: in which there is the code of parallel for implementation.
I have done three tests (1, 10 and 40 million number ranges) for each function, you can find the results and the plots in the folder "plot" where there are three subfolders in which you can find the scalability and speedup plot. Moreover, in the subfolder "results" you can find the time for each test from 1 to 250 worker.
The master_worker was tested with the double of tasks than the worker instantiated.
The results of the parallel for presents take are done using dynamic scheduling with a chunk size of 1.

