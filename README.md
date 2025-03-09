
# Multilevel Queue Scheduling Report
---

## 1. Introduction
Multilevel queue scheduling is used when processes are categorized into different priority classes. In this project, I implemented a multilevel queue scheduling algorithm with four queues, each assigned a different scheduling algorithm:

* q0 - Round Robin (RR) (Highest priority)
* q1 - Shortest Job First (SJF)
* q2 - Shortest Job First (SJF)
* q3 - First-In-First-Out (FIFO) (Lowest priority)

Each queue gets a 20-second time quantum before the CPU switches to the next queue. The goal is to efficiently schedule processes based on priority and execution time while analyzing waiting time and turnaround time.

---

## 2. Implementation Details
### 2.1. Code Structure
The program is implemented in C++ with the following files:

- `mlq.h` – Defines data structures (process, queues, CPU, scheduler)
- `mlq.cpp` – Implements scheduling logic, process handling, and CPU execution
- `main.cpp` – Handles user input and runs the scheduler

### 2.2. Process Scheduling
Processes are added to one of the four queues based on user input. The scheduling mechanism works as follows:

* Round Robin (RR) [Queue 0]
    - Uses a time quantum (5 seconds per process slice).
    - If a process exceeds the quantum, it is re-enqueued.


* Shortest Job First (SJF) [Queue 1 & Queue 2]
    - The process with the shortest burst time is selected first.
    - This minimizes waiting time for short processes.


* First-In-First-Out (FIFO) [Queue 3]
    - Processes are executed in the order they arrive (no preemption).
    - Useful for batch processing tasks.

Each queue runs until it either finishes all processes or reaches the 20-second time quantum before switching to the next queue.

---

## 3. Results and Performance Analysis
### 3.1. Test Setup
The program was tested with 100 randomly generated processes and user-input processes. The results were logged in log.csv, containing:

- Queue ID
- Process ID
- Waiting Time
- Turnaround Time


### 3.2. Observations
- Round Robin (q0) handled short processes well but caused higher waiting time for long processes.
- SJF queues (q1 & q2) had the lowest waiting time since shorter processes finished quickly.
- FIFO queue (q3) was inefficient for interactive tasks, as longer processes had to wait until earlier ones finished.

### 3.3. Average Waiting Time & Turnaround Time
|Queue	 | scheduling Type |Average Waiting Time|Average Turnaround Time|
|--------|-----------------|--------------------|-----------------------|
|q0      | Round Robin (RR)|	High            |	Moderate            |
|q1	     |Shortest Job First (SJF)|	Low         |	Low                 |
|q2	     |Shortest Job First (SJF)|	Low         |	Low                 |
|q3	     |First-In-First-Out (FIFO)|	High    |	High                |

---

## 4. Pros & Cons of Each Scheduling Algorithm
### 4.1. Round Robin (RR)
##### ✅ Pros:
- Fair: Each process gets CPU time.
- Prevents starvation since every process runs at some point.
- Good for time-sharing systems (e.x., interactive user tasks).

##### ❌ Cons:
- Higher waiting time if too many processes are in the queue.
- Inefficient for short processes, as context switching overhead increases.

### 4.2. Shortest Job First (SJF)
##### ✅ Pros:
- Minimizes waiting time by selecting the shortest job first.
- Efficient for batch processing with predictable execution times.

##### ❌ Cons:
- Starvation risk: Long processes might never get CPU time if shorter jobs keep arriving.
- Requires knowing burst times in advance, which may not always be possible.

### 4.3. First-In-First-Out (FIFO)
##### ✅  Pros:
- Simple and easy to implement.
- Good for non-interactive background tasks (e.g., printing, batch jobs).

##### ❌ Cons:
- Poor response time for interactive tasks.
- Long processes delay all other jobs (convoy effect).

---

## 5. Conclusion
The multilevel queue scheduling algorithm effectively separates different scheduling needs based on process priority. The results showed:

- SJF was the most efficient in minimizing waiting time.
- RR provided fairness but had higher waiting times.
- FIFO was simplest but inefficient for mixed workloads.


### Limitations & Future Improvements
- No dynamic queue movement: Processes do not move between queues based on behavior.
- Lack of aging for lower-priority processes: Starvation can still occur in some cases.
- Performance overhead due to re-enqueuing in RR can be optimized using a circular queue.
- A future improvement could be a priority boost mechanism, where long-waiting processes move to a higher-priority queue over time.
- Can be implemented in multiple threads to enter processes into queues while the CPU processes them.

---

## Author

Name: **Bhasura Jayaweera**
Registration number: **2023/CS/080**
Index number: **23000805**
Email Address:  2023cs080@stu.ucsc.cmb.ac.lk

---

### Testing Environment:
This program was tested on **WSL** (Windows Subsystem for **Linux**) version 2 environment with Windows 11 Operating System.

---

### How to Execute the Program:

* Ensure that all the files are in the same directory.
* Compile the program using the following command:
    `g++ -o mlq main.cpp mlq.cpp`
* Run the program using:
    `./mlq`


### Special Notes:
- The program prints the process and process table.
- The program creates a file `log.csv` with process logs.
     - csv order (`QueueID `, `PID` , `Waiting time` , `Turnaround time`)
- A simple python script `analyze.py` is included to analyze the `log.csv` file.


