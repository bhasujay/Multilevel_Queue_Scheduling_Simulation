#include <iostream>
#include "mlq.h"


int main()
{
    CPU MyCPU = CPU();
    CPUScheduler MyScheduler = CPUScheduler();

    int choice;
    std::cout << "Choose process input method:\n";
    std::cout << "1. Manual Entry\n";
    std::cout << "2. Randomly Generate 100 Processes\n";
    std::cout << "Enter choice: ";
    std::cin >> choice;

    if (choice == 1)
        manualEntry(MyScheduler);
    else
        for (int i = 0; i < 100; i++)
            randomEntry(MyScheduler);

    MyScheduler.startSheduling(MyCPU);

    printLog(MyScheduler.log);

    MyScheduler.freeQueues();

    std::cout << "Press Enter to exit...";
    std::cin.ignore(); 
    std::cin.get();

}