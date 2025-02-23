#pragma once

#include <iostream>
#include <random>
#include <string>
#include <chrono>
#include <thread>
#include <map>
#include <fstream>

// Constants
// Queue Ids
#define RR 0
#define SJF 1
#define FIFO 3

// Process States
#define READY 0
#define RUNNING 1
#define BLOCKED 2
#define FINISHED 3

#define TIME_QUANTUM 20
#define RR_TIME_QUANTUM 5
#define TIME_FACTOR 0


#define LINE std::cout << "--------------------------------------------------------------------------------\n"
#define BOLDLINE std::cout << "################################################################################\n"


// Process structure
struct Process
{
    int priority;
    int burstTime;
    int pid;
    int state;

    Process(int pid, int priority, int burstTime, int state = READY);
};

// Node to hold a Process structure
struct PNode
{
    Process* process;
    PNode* next;
    PNode* prev;

    PNode(Process* process);
};

// Queue for Process Nodes
// This is base Queue structure
struct Queue
{   
    PNode* head;
    PNode* tail;
    int size;
    int id;

    Queue(int id);
    virtual void enqueue(Process* process);
    Process* dequeue();
    bool isEmpty();
    void print();
    std::string queueName();
};

// First In First Out Queue
// Inherits from Queue
struct FIFOQueue : public Queue
{
    FIFOQueue(int id);
    void enqueue(Process* process) override;
};

// Shortest Job First Queue
// Inherits from Queue
struct SJFQueue : public Queue
{
    SJFQueue(int id);
    void enqueue(Process* process) override;
};

// CPU Structure
struct CPU
{
    long counter;

    CPU();
    void run(Process* process);
};

// Multi-Level Queue Scheduler
struct CPUScheduler
{
    Queue* queues[4];
    std::map<int, std::tuple<int, int, int, int>> log;

    CPUScheduler();
    void startSheduling(CPU& cpu);
    void freeQueues();
};


// Functions
int randomNum();
void randomEntry(CPUScheduler& Scheduler);
void startLog(std::map<int, std::tuple<int, int, int, int>>& log, int pid, int queueID, int startTime, int burstTime);
void endLog(std::map<int, std::tuple<int, int, int, int>>& log, int pid, int endTime);
void printLog(const std::map<int, std::tuple<int, int, int, int>>& log);
void manualEntry(CPUScheduler& Scheduler);