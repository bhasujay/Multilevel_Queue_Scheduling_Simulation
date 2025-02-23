#include "mlq.h"

// Process structure
Process::Process(int pid, int priority, int burstTime, int state):
        pid(pid),
        priority(priority),
        burstTime(burstTime),
        state(state) {};

// Node to hold a Process structure
PNode::PNode(Process* process):
        process(process),
        next(nullptr),
        prev(nullptr) {};


// Queues for Process Nodes
Queue::Queue(int id):
        head(nullptr),
        tail(nullptr),
        size(0),
        id(id) {};

void Queue::enqueue(Process* process)
{
    return;
}

Process* Queue::dequeue()
{
    if (head == nullptr)
        return nullptr;

    if (head == tail)
    {
        Process* process = head->process;
        delete head;
        head = tail = nullptr;
        size--;
        return process;
    }

    PNode* temp = head;
    head = head->next;
    head->prev = nullptr;
    Process* process = temp->process;
    delete temp;
    size--;

    return process;
}

bool Queue::isEmpty()
{
    return head == nullptr && tail == nullptr;
}

void Queue::print()
{
    switch (id)
    {
        case RR:
        std::cout << "   Round Robin:\n\t";
        break; 
        case SJF:
        std::cout << "   Shortest Job First:\n\t";
        break;
        case FIFO:
        std::cout << "   First In First Out:\n\t";
        break;
    }
    
    PNode* temp = head;
    while (temp)
    {
        std::cout << temp->process->pid << "(" << temp->process->burstTime << ")";
        if (temp->next != nullptr)
            std::cout << " <- ";
        temp = temp->next;
    }
    std::cout << std::endl << std::endl; 
}

std::string Queue::queueName()
{
    switch (id)
    {
        case RR:
        return "Round Robin";
        case SJF:
        return "Shortest Job First";
        case FIFO:
        return "First In First Out";
    }
    return "";
}

// Sub classes of queues
FIFOQueue::FIFOQueue(int id): 
        Queue(id) {};

void FIFOQueue::enqueue(Process* process)
{
    PNode* newNode = new PNode(process);

    if (head == nullptr)
    {
        head = tail = newNode;
        size++;
        return;
    }

    tail->next = newNode;
    newNode->prev = tail;
    tail = newNode;
    size++;
}

SJFQueue::SJFQueue(int id): 
        Queue(id) {};

void SJFQueue::enqueue(Process* process)
{
    PNode* newNode = new PNode(process);

    if (head == nullptr)
    {
        head = tail = newNode;
        size++;
        return;
    }

    if (process->burstTime < head->process->burstTime)
    {
        head->prev = newNode;
        newNode->next = head;
        head = newNode;
        size++;
        return;
    }

    PNode* temp = head;

    while (temp->next && process->burstTime > temp->next->process->burstTime)
        temp = temp->next;

    if (temp->next == nullptr)
    {
        tail->next = newNode;
        newNode->prev = tail;
        tail = newNode;
        size++;
        return;
    }

    newNode->next = temp->next;
    temp->next->prev = newNode;
    temp->next = newNode;
    newNode->prev = temp;

    size++;
}


// CPU structure
CPU::CPU():
        counter(0) {};

void CPU::run(Process* process)
{
    process->state = RUNNING;
    counter += process->burstTime;
    std::cout << "\tProcess " << process->pid << " RUNNING - "<< process->burstTime;
    std::this_thread::sleep_for(std::chrono::milliseconds((int)(process->burstTime * 1000 * TIME_FACTOR)));
}

// CPU Scheduler structure
CPUScheduler::CPUScheduler()
{
    queues[0] = new FIFOQueue(RR);   // For q0 - Round Robin (RR) (The same FIFOQueue is used for RR, Scheduler will handle the time quantum)
    queues[1] = new SJFQueue(SJF);    // For q1 - Shortest Job First (SJF)
    queues[2] = new SJFQueue(SJF);    // For q2 - Shortest Job First (SJF)
    queues[3] = new FIFOQueue(FIFO);   // For q3 - First In First Out (FIFO)
}

void CPUScheduler::startSheduling(CPU& cpu)
{
    int allEmpty = 0;

    // main loop
    while (allEmpty < 4)
    {
        BOLDLINE;
        // loop through all queues
        for (int i = 0; i < 4; i++)
        {
            LINE;
            std::cout << "q" << i << " Queue - " << queues[i]->queueName() << std::endl; // display the working queue
            
            if (queues[i]->isEmpty())   // check if the queue is empty
            {
                allEmpty++;
                std::cout << "   QUEUE IS EMPTY (switching to next one)" << std::endl;
                continue;
            }
            queues[i]->print();
            allEmpty = 0;
            int timeQuantum = TIME_QUANTUM; // time quantum for the queue

            // loop inside a queue
            while (!queues[i]->isEmpty() && timeQuantum > 0)
            {

                // for the Round Robin queue
                if (queues[i]->id == RR)
                {
                    Process* process = queues[i]->dequeue();

                    if (process->state == READY) // log the start time
                        startLog(log, process->pid, i, cpu.counter, process->burstTime);

                    int time = process->burstTime;

                    if (time > timeQuantum)
                    {
                        if (timeQuantum > RR_TIME_QUANTUM)
                        {
                            process->burstTime = RR_TIME_QUANTUM;
                            time -= RR_TIME_QUANTUM;
                            cpu.run(process);
                            process->burstTime = time;
                            process->state = BLOCKED;
                            std::cout << " BLOCKED\n";
                            queues[i]->enqueue(process);
                            timeQuantum -= RR_TIME_QUANTUM;
                        }
                        else 
                        {
                            process->burstTime = timeQuantum;
                            time -= timeQuantum;
                            cpu.run(process);
                            process->burstTime = time;
                            process->state = BLOCKED;
                            std::cout << " BLOCKED\n";
                            queues[i]->enqueue(process);
                            timeQuantum = 0;
                        }
                    }
                    else
                    {
                        if (time > RR_TIME_QUANTUM)
                        {
                            process->burstTime = RR_TIME_QUANTUM;
                            time -= RR_TIME_QUANTUM;
                            cpu.run(process);
                            process->burstTime = time;
                            process->state = BLOCKED;
                            std::cout << " BLOCKED\n";
                            queues[i]->enqueue(process);
                            timeQuantum -= RR_TIME_QUANTUM;
                        }
                        else
                        {
                            cpu.run(process);
                            process->state = FINISHED;
                            std::cout << " FINISHED\n";
                            endLog(log, process->pid, cpu.counter); // log the ternaround time
                            delete process;
                            process = nullptr;
                            timeQuantum -= time;
                        }
                    }
                }
            
                // for the shortest job first queues and the FIFO queue
                else if (queues[i]->id == SJF || queues[i]->id == FIFO)
                {
                    Process* process = queues[i]->dequeue();

                    if (process->state == READY) // log the start time
                        startLog(log, process->pid, i, cpu.counter, process->burstTime);

                    int time = process->burstTime;

                    if (time > timeQuantum)
                    {
                        process->burstTime = timeQuantum;
                        time -= timeQuantum;
                        cpu.run(process);
                        process->burstTime = time;
                        process->state = BLOCKED;
                        std::cout << " BLOCKED\n";
                        queues[i]->enqueue(process);
                        timeQuantum = 0;
                    }
                    else
                    {
                        cpu.run(process);
                        process->state = FINISHED;
                        std::cout << " FINISHED\n";
                        endLog(log, process->pid, cpu.counter);
                        delete process;
                        process = nullptr;
                        timeQuantum -= time;
                    }
                }
            }

            if (queues[i]->isEmpty())
                std::cout << "\n   Queue is finished. Remaining time - " << timeQuantum << std::endl;
            else
                std::cout << "\n   " << TIME_QUANTUM << "s ended.\n";
        }
    }
    std::cout << "\nAll processes are finished!\n" << std::endl;
}

void CPUScheduler::freeQueues()
{
    for (int i = 0; i < 4; i++)
    {
        while (!queues[i]->isEmpty()) // Just in case if there are processes remain in queues
        {
            Process* process = queues[i]->dequeue();
            delete process;
            process = nullptr;
        }
        delete queues[i];
        std::cout << "Queue " << i << " is deleted\n";
        queues[i] = nullptr;
    }
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////

// random number generator
int randomNum()
{
    static bool seed = false;

    if(seed)
        return rand();

    srand(time(NULL));
    seed = true;
    return rand();
}

// randowm process entry
void randomEntry(CPUScheduler& Scheduler)
{
    static int pid = 0;
    int priority = randomNum() % 10;
    int burstTime = randomNum() % 25;

    Scheduler.log[pid] = std::make_tuple(0, 0, 0, 0);

    int qID = randomNum() % 4;

    Process* p = new Process(pid++, priority, ++burstTime);

    Scheduler.queues[qID]->enqueue(p);

    char qName[4][20] = {"RR", "SJF", "SJF", "FIFO"};

    std::cout << "Queue" << qName[qID] << "-> Process( PID-" << p->pid << ", BurstTime-" << p->burstTime << ", Priority-" << p->priority << ", state-READY)\n";
}

// logger
void startLog(std::map<int, std::tuple<int, int, int, int>>& log, int pid, int queueID, int startTime, int burstTime)
{
    std::get<0>(log[pid]) = queueID;
    std::get<1>(log[pid]) = startTime;
    std::get<2>(log[pid]) = burstTime;
}

void manualEntry(CPUScheduler& Scheduler)
{
    int numProcesses;
    std::cout << "Enter the number of processes: ";
    std::cin >> numProcesses;

    for (int i = 0; i < numProcesses; i++)
    {
        int qID, burstTime, priority;
        std::cout << "\nEnter details for Process " << i + 1 << ":\n";

        std::cout << "Queue ID (0: RR, 1: SJF, 2: SJF, 3: FIFO): ";
        std::cin >> qID;
        while (qID < 0 || qID > 3)
        {
            std::cout << "Invalid Queue ID! Enter again (0-3): ";
            std::cin >> qID;
        }

        std::cout << "Burst Time: ";
        std::cin >> burstTime;

        std::cout << "Priority (optional, default 0): ";
        std::cin >> priority;

        Process* p = new Process(i, priority, burstTime);
        Scheduler.queues[qID]->enqueue(p);

        std::cout << "Process " << i << " added to Queue " << qID << " successfully!\n";
    }
}

void endLog(std::map<int, std::tuple<int, int, int, int>>& log, int pid, int endTime)
{
    std::get<3>(log[pid]) = endTime;
}

void printLog(const std::map<int, std::tuple<int, int, int, int>>& log)
{
    std::cout << "Log Entries:\n";
    std::cout << "-------------------------------------------------\n";
    std::cout << "|PID\t|Start\t|Burst\t|Turnaround |Waiting\t|\n";
    std::cout << "-------------------------------------------------\n";

    
    for (const auto& entry : log) {
        int pid = entry.first;
        auto& values = entry.second;

        int start = std::get<1>(values);
        int burst = std::get<2>(values);
        int end = std::get<3>(values);
        
        std::cout << "|" << pid << "\t|" << start << "\t|" << burst << "\t|" << end << "\t    |" << end-burst << "  \t|\n";
    }
    
    std::cout << "-------------------------------------------------\n";

    std::string filename  = "log.csv";
    std::ofstream csvFile(filename);
    char qName[4][20] = {"RR", "SJF", "SJF", "FIFO"};

    if (csvFile.is_open())
    {
        csvFile << "QueueID,PID,Waiting time,Turnaround time\n";

        for (const auto& entry : log) {
            int pid = entry.first;
            auto& values = entry.second;

            int qID = std::get<0>(values);
            int burst = std::get<2>(values);
            int end = std::get<3>(values);
            
            csvFile << qName[qID] << "," << pid << "," << (end - burst) << "," << end << "\n";
        }

        csvFile.close();
        std::cout << "Log entries saved to " << filename << "\n";
    }
    else
        std::cerr << "Failed to open file for writing: " << filename << "\n";

}



