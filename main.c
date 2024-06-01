#include "main.h"

Process processes[MAX_PROCESSES];
Process originalProcesses[MAX_PROCESSES];
int processMap[PID_MAX + 1];
int processCount;

int main() {
    int processNum;
    int scheduleNum;
    int timeQuantum;

    printf("Enter the number of processes: ");
    scanf("%d", &processNum);

    if (processNum > MAX_PROCESSES) {
        printf("Maximum number of processes is %d\n", MAX_PROCESSES);
        return 1;
    }

    printf("Enter time quantum: ");
    scanf("%d", &timeQuantum);
    printf("\n\n");
    
    createProcess(processNum);
    printf("Before Scheduling:\n");
    printProcesses();
    printf("\n\n");

    while (1) {
        printf("==============Scheduling Algorithm=================\n");
        printf("1. FCFS\n");
        printf("2. Non-preemptive SJF\n");
        printf("3. Preemptive SJF\n");
        printf("4. Non-preemptive Priority\n");
        printf("5. Preemptive Priority\n");
        printf("6. Round Robin\n");
        printf("7. Exit\n");
        printf("===================================================\n");

        printf(">  ");
        scanf("%d", &scheduleNum);

        resetProcesses(); // 초기 상태로 되돌림

        switch (scheduleNum) {
            case 1:
                fcfs();
                break;
            case 2:
                nonPreemptiveSJF();
                break;
            case 3:
                preemptiveSJF();
                break;
            case 4:
                nonPreemptivePriority();
                break;
            case 5:
                preemptivePriority();
                break;
            case 6:
                roundRobin(timeQuantum);
                break;
            case 7:
                break;
            default:
                break;
        }
        if(scheduleNum == 7){
            break;
        }

        printf("\nAfter Scheduling:\n");
        printProcesses();
    }

    return 0;
}
