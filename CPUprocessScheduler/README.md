# Files #
- main.cpp  (containing main logic of program)
- makefile  (making executable from main.cpp and parser.h)
- parser.h  (parses the input data and stores into variables in format to make it ready for further processing)
- testcases (directory having testcases files, their usage is explained in README.md inside testcases folder)

## Overview ##
An implementation of various CPU scheduling algorithms in C++. The algorithms included are First Come First Serve (FCFS), Round Robin (RR), Shortest Process Next (SPN), Shortest Remaining Time (SRT), Highest Response Ratio Next (HRRN), Feedback (FB) and Aging.

## Usage ##
1- Clone the repository

2- Install g++ compiler and make
```bash
sudo apt-get install g++ make
```
3- Compile the code using `make` command

4- Run the executable file

## Input Format ##
- Line 1: "trace" or "stats"
- Line 2: a comma-separated list telling which CPU scheduling policies to be analyzed/visualized along with
their parameters, if applicable. Each algorithm is represented by a number as listed in the
introduction section and as shown in the attached testcases.
Round Robin and Aging have a parameter specifying the quantum q to be used. Therefore, a policy
entered as 2-4 means Round Robin with q=4. Also, policy 8-1 means Aging with q=1. <br>
Quantum Number is neede only in '2' and '8' Algorithms
 1. FCFS (First Come First Serve)
 2. RR (Round Robin)
 3. SPN (Shortest Process Next)
 4. SRT (Shortest Remaining Time)
 5. HRRN (Highest Response Ratio Next)
 6. FB-1, (Feedback where all queues have (quantum)q=1)
 7. FB-2^i, (Feedback where q= 2^i , where i is priority level)
 8. Aging
- Line 3: An integer specifying the last instant to be used in your simulation and to be shown on the timeline.
- Line 4: An integer specifying the number of processes to be simulated.
- Line 5: Start of description of processes. Each process is to be described on a separate line. For algorithms 1 through 7, each process is described using a comma-separated list specifying:

    1- String specifying a process name\
    2- Arrival Time\
    3- Service Time

- **Note:** For Aging algorithm (algorithm 8), each process is described using a comma-separated list specifying:

    1- String specifying a process name\
    2- Arrival Time\ 
    3- Priority (Also equal to waiting time for a process)

**Important:** Processes are assumed to be sorted based on the priority. If two processes have the same priority, then the one with the lower arrival time is assumed to arrive first. Otherwise, unexpected behavior <br> <br>
**Important:** Ensure that your input processing completes up to the last instant for all algorithms. Failure to do so may result in unexpected behavior, such as segmentation faults or incorrect results, especially in algorithms like Round Robin and MLFQ

> Check the attached [testcases](https://github.com/NITINSPATEL/Projects/tree/main/CPUprocessScheduler/testcases) for more details of the input format.
