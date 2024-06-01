#include "main.h"


int generateUniquePid() {
    int pid;
    do {
        pid = rand() % (PID_MAX - PID_MIN + 1) + PID_MIN;
    } while (processMap[pid] != 0);
    return pid;
}



void createProcess(int processNum) {
    srand(time(NULL));
    for (int i = 1; i < processNum + 1; i++) {
        int io = rand() % 100 + 1 > 80 ? 1 : 0;
        int pid = generateUniquePid();
        originalProcesses[i].pid = pid;
        processMap[pid] = i;

        originalProcesses[i].cpuBurstTime = rand() % 10 + 1;

        if (originalProcesses[i].cpuBurstTime == 1) io = 0;
        originalProcesses[i].ioBurstTiming = io ? rand() % (originalProcesses[i].cpuBurstTime - 1) + 1 : 0;
        originalProcesses[i].ioBurstTime = io ? rand() % 3 + 1 : 0;

        originalProcesses[i].arrivalTime = rand() % 10;
        originalProcesses[i].waitingStartTime = originalProcesses[i].arrivalTime;
        originalProcesses[i].priority = rand() % 10;
        originalProcesses[i].cpuRemainingTime = originalProcesses[i].cpuBurstTime;
        originalProcesses[i].ioRemainingTime = originalProcesses[i].ioBurstTime;
        originalProcesses[i].waitingTime = 0;
        originalProcesses[i].turnaroundTime = 0;
    }
    processCount = processNum;

    for (int i = 1; i < processCount + 1; i++) {
        processes[i] = originalProcesses[i];
    }
}

void resetProcesses() {
    for (int i = 1; i < processCount + 1; i++) {
        processes[i] = originalProcesses[i];
    }
}


void printProcesses() {
    printf("PID\tPriority\tArrival\tCPU Burst\tI/O Burst\tI/O Timing\tWaiting\tTurnaround\n");
    for (int i = 1; i < processCount + 1; i++) {
        printf("%d\t%d\t\t%d\t%d\t\t%d\t\t%d\t\t%d\t%d\n",
               processes[i].pid,
               processes[i].priority,
               processes[i].arrivalTime,
               processes[i].cpuBurstTime,
               processes[i].ioBurstTime,
               processes[i].ioBurstTiming,
               processes[i].waitingTime,
               processes[i].turnaroundTime);
    }
}

void evaluate(){
    int turnaroundTime=0;
    int waitingTime=0;

    for(int i = 1; i<processCount+1; i++){
        turnaroundTime += processes[i].turnaroundTime;
        waitingTime += processes[i].waitingTime;
    }

    printf("Average Turnaround Time: %f\n", (double)turnaroundTime/(double)processCount);
    printf("Average Waiting Time: %f\n", (double)waitingTime/(double)processCount);
}


void ganttChart(int chart[], int size) {
    printf("\nGantt Chart:\n");
    printf("|");
    for (int i = 0; i < size; i++) {
        if (chart[i] == 0) {
            printf(" IDLE |");
        } else {
            printf(" P%d |", chart[i]);
        }
    }
    printf("\n");
    for (int i = 0; i < size; i++) {
        printf(" %2d  ", i);
    }
    printf("\n");
}