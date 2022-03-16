# COMP30023 Project 1 - CPU Scheduler
A simulator of a CPU scheduler that can handle multiple processors and parallelisable processes. Implemented with the shortest-time-remaining algorithm to schedule processes. Also includes statistics on performance.
<img width="762" alt="Screen Shot 2022-03-16 at 5 03 09 pm" src="https://user-images.githubusercontent.com/21867116/158530185-e80cb599-7b58-4230-8846-05f03f50bab0.png">


## Usage
### Compiling
`make clean && make`
### Running
`./allocate -f <path/to/input> [-p] <numberOfProcessors> [-c]`  
  
-f **_filename_** will specify the name of the file describing the processes.  
-p **_processors_** where processors is one of {1,2,N}, N ≤ 1024.  
-c an optional parameter, when provided, invokes a different scheduling algorithm aimed at improving performance.
