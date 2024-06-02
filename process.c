#include "main.h"


int generateUniquePid() {
    int pid;
    do {
        pid = rand() % (PID_MAX - PID_MIN + 1) + PID_MIN;
    } while (processMap[pid] != 0);
    return pid;
}

void resetProcesses() {
    for (int i = 1; i < processCount + 1; i++) {
        processes[i] = originalProcesses[i];
    }
}

/////////////////////////////////////////// 디버깅 용
// void createProcess() {
//     processCount = 4;

//     // 프로세스 1
//     originalProcesses[1].pid = 5527;
//     processMap[5527] = 1;
//     originalProcesses[1].priority = 5;
//     originalProcesses[1].arrivalTime = 5;
//     originalProcesses[1].cpuBurstTime = 2;
//     originalProcesses[1].ioBurstTime = 0;
//     originalProcesses[1].ioBurstTiming = 0;
//     originalProcesses[1].waitingStartTime = 5;
//     originalProcesses[1].cpuRemainingTime = 2;
//     originalProcesses[1].ioRemainingTime = 0;
//     originalProcesses[1].waitingTime = 0;
//     originalProcesses[1].turnaroundTime = 0;

//     // 프로세스 2
//     originalProcesses[2].pid = 9166;
//     processMap[9166] = 2;
//     originalProcesses[2].priority = 1;
//     originalProcesses[2].arrivalTime = 6;
//     originalProcesses[2].cpuBurstTime = 9;
//     originalProcesses[2].ioBurstTime = 1;
//     originalProcesses[2].ioBurstTiming = 3;
//     originalProcesses[2].waitingStartTime = 6;
//     originalProcesses[2].cpuRemainingTime = 9;
//     originalProcesses[2].ioRemainingTime = 1;
//     originalProcesses[2].waitingTime = 0;
//     originalProcesses[2].turnaroundTime = 0;

//     // 프로세스 3
//     originalProcesses[3].pid = 6953;
//     processMap[6953] = 3;
//     originalProcesses[3].priority = 5;
//     originalProcesses[3].arrivalTime = 4;
//     originalProcesses[3].cpuBurstTime = 3;
//     originalProcesses[3].ioBurstTime = 3;
//     originalProcesses[3].ioBurstTiming = 2;
//     originalProcesses[3].waitingStartTime = 4;
//     originalProcesses[3].cpuRemainingTime = 3;
//     originalProcesses[3].ioRemainingTime = 3;
//     originalProcesses[3].waitingTime = 0;
//     originalProcesses[3].turnaroundTime = 0;

//     // 프로세스 4
//     originalProcesses[4].pid = 2587;
//     processMap[2587] = 4;
//     originalProcesses[4].priority = 0;
//     originalProcesses[4].arrivalTime = 9;
//     originalProcesses[4].cpuBurstTime = 3;
//     originalProcesses[4].ioBurstTime = 0;
//     originalProcesses[4].ioBurstTiming = 0;
//     originalProcesses[4].waitingStartTime = 9;
//     originalProcesses[4].cpuRemainingTime = 3;
//     originalProcesses[4].ioRemainingTime = 0;
//     originalProcesses[4].waitingTime = 0;
//     originalProcesses[4].turnaroundTime = 0;

//     // originalProcesses 배열을 processes 배열로 복사
//     for (int i = 1; i <= processCount; i++) {
//         processes[i] = originalProcesses[i];
//     }
// }
///////////////////////////////////////////////

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


// void ganttChart(int chart[], int size) {
//     printf("\nGantt Chart:\n");
//     printf("|");
//     for (int i = 0; i < size; i++) {
//         if (chart[i] == 0) {
//             printf(" IDLE |");
//         } else {
//             printf(" P%d |", chart[i]);
//         }
//     }
//     printf("\n");
//     for (int i = 0; i < size; i++) {
//         printf(" %4d  ", i+1);
//     }
//     printf("\n");
// }
void ganttChart(int chart[], int size) {
    printf("\nGantt Chart:\n");
    printf("|");
    for (int i = 0; i < size; i++) {
        if (chart[i] == 0) {
            printf(" IDLE  |");  // "IDLE"을 6글자로 맞춤
        } else {
            printf(" P%4d |", chart[i]);
        }
    }
    printf("\n");
    for (int i = 0; i < size; i++) {
        printf("  %5d ", i + 1);
    }
    printf("\n");
}
