#include "main.h"



void sortReadyQueueByCpuBurstTime(int queue[], int* front, int* rear){
    for (int i = *front; i != *rear; i = (i + 1) % (MAX_QUEUE_SIZE + 1)) {
        for (int j = (i + 1) % (MAX_QUEUE_SIZE + 1); j != *rear; j = (j + 1) % (MAX_QUEUE_SIZE + 1)) {
            if (processes[processMap[queue[i]]].cpuRemainingTime > processes[processMap[queue[j]]].cpuRemainingTime) {
                int temp = queue[i];
                queue[i] = queue[j];
                queue[j] = temp;
            }
        }
    }
}

void sortReadyQueueByPriority(int queue[], int* front, int* rear){
    for (int i = *front; i != *rear; i = (i + 1) % (MAX_QUEUE_SIZE + 1)) {
        for (int j = (i + 1) % (MAX_QUEUE_SIZE + 1); j != *rear; j = (j + 1) % (MAX_QUEUE_SIZE + 1)) {
            if (processes[processMap[queue[i]]].priority > processes[processMap[queue[j]]].priority) {
                int temp = queue[i];
                queue[i] = queue[j];
                queue[j] = temp;
            }
        }
    }
}

void io(int queue[], int *front, int *rear) {
    for (int i = *front; i != *rear; i = (i + 1) % (MAX_QUEUE_SIZE + 1)) {
        processes[processMap[queue[i]]].ioRemainingTime--;
    }
}


void fcfs() {
    int currentPid = -1;
    int currentTime = 0;

    int chart[1000];

    int readyQueue[MAX_QUEUE_SIZE];
    int waitingQueue[MAX_QUEUE_SIZE];

    int readyQueueFront = 0, readyQueueRear = 0;
    int waitingQueueFront = 0, waitingQueueRear = 0;

    int completed = 0;

    while (completed < processCount) {
        if (currentPid != -1 && processes[processMap[currentPid]].ioBurstTime != 0) {
            if (processes[processMap[currentPid]].ioBurstTiming == processes[processMap[currentPid]].cpuBurstTime - processes[processMap[currentPid]].cpuRemainingTime) {
                enqueue(waitingQueue, &waitingQueueFront, &waitingQueueRear, currentPid);

                for (int i = waitingQueueFront; i != waitingQueueRear; i = (i + 1) % (MAX_QUEUE_SIZE + 1)) {
                    for (int j = (i + 1) % (MAX_QUEUE_SIZE + 1); j != waitingQueueRear; j = (j + 1) % (MAX_QUEUE_SIZE + 1)) {
                        if (processes[processMap[waitingQueue[i]]].ioRemainingTime > processes[processMap[waitingQueue[j]]].ioRemainingTime) {
                            int temp = waitingQueue[i];
                            waitingQueue[i] = waitingQueue[j];
                            waitingQueue[j] = temp;
                        }
                    }
                }

                processes[processMap[currentPid]].waitingStartTime = currentTime;
                currentPid = -1;
            }
        }

        for (int i = 1; i < processCount + 1; i++) {
            if (processes[i].arrivalTime == currentTime) {
                enqueue(readyQueue, &readyQueueFront, &readyQueueRear, processes[i].pid);
            }
        }

        for (int i = waitingQueueFront; i != waitingQueueRear; i = (i + 1) % (MAX_QUEUE_SIZE + 1)) {
            if (processes[processMap[waitingQueue[i]]].ioRemainingTime == 0) {
                int pid = dequeue(waitingQueue, &waitingQueueFront, &waitingQueueRear);
                enqueue(readyQueue, &readyQueueFront, &readyQueueRear, pid);
            }
        }

        if (currentPid == -1 && readyQueueFront != readyQueueRear) {
            currentPid = dequeue(readyQueue, &readyQueueFront, &readyQueueRear);
            processes[processMap[currentPid]].waitingTime += currentTime - processes[processMap[currentPid]].waitingStartTime;
        }

        chart[currentTime] = currentPid;

        if (currentPid != -1) {
            processes[processMap[currentPid]].cpuRemainingTime--;
            if (processes[processMap[currentPid]].cpuRemainingTime == 0) {
                processes[processMap[currentPid]].turnaroundTime = (currentTime + 1 - processes[processMap[currentPid]].arrivalTime);
                currentPid = -1;
                completed++;
            }
        } else {
            chart[currentTime] = 0; // IDLE
        }

        io(waitingQueue, &waitingQueueFront, &waitingQueueRear);

        currentTime++;
    }

    ganttChart(chart, currentTime);
    evaluate();
}




void nonPreemptiveSJF(){
    int currentPid = -1;
    int currentTime = 0;

    int chart[1000];

    int readyQueue[MAX_QUEUE_SIZE];
    int waitingQueue[MAX_QUEUE_SIZE];

    int readyQueueFront = 0, readyQueueRear = 0;
    int waitingQueueFront = 0, waitingQueueRear = 0;

    int completed = 0;

    while (completed < processCount) {
        if (currentPid != -1 && processes[processMap[currentPid]].ioBurstTime != 0) {
            if (processes[processMap[currentPid]].ioBurstTiming == processes[processMap[currentPid]].cpuBurstTime - processes[processMap[currentPid]].cpuRemainingTime) {
                enqueue(waitingQueue, &waitingQueueFront, &waitingQueueRear, currentPid);

                for (int i = waitingQueueFront; i != waitingQueueRear; i = (i + 1) % (MAX_QUEUE_SIZE + 1)) {
                    for (int j = (i + 1) % (MAX_QUEUE_SIZE + 1); j != waitingQueueRear; j = (j + 1) % (MAX_QUEUE_SIZE + 1)) {
                        if (processes[processMap[waitingQueue[i]]].ioRemainingTime > processes[processMap[waitingQueue[j]]].ioRemainingTime) {
                            int temp = waitingQueue[i];
                            waitingQueue[i] = waitingQueue[j];
                            waitingQueue[j] = temp;
                        }
                    }
                }

                processes[processMap[currentPid]].waitingStartTime = currentTime;
                currentPid = -1;
            }
        }

        for (int i = 1; i < processCount + 1; i++) {
            if (processes[i].arrivalTime == currentTime) {
                enqueue(readyQueue, &readyQueueFront, &readyQueueRear, processes[i].pid);
                sortReadyQueueByCpuBurstTime(readyQueue, &readyQueueFront, &readyQueueRear);
            }
        }

        for (int i = waitingQueueFront; i != waitingQueueRear; i = (i + 1) % (MAX_QUEUE_SIZE + 1)) {
            if (processes[processMap[waitingQueue[i]]].ioRemainingTime == 0) {
                int pid = dequeue(waitingQueue, &waitingQueueFront, &waitingQueueRear);
                enqueue(readyQueue, &readyQueueFront, &readyQueueRear, pid);
                sortReadyQueueByCpuBurstTime(readyQueue, &readyQueueFront, &readyQueueRear);
            }
        }

        if (currentPid == -1 && readyQueueFront != readyQueueRear) {
            currentPid = dequeue(readyQueue, &readyQueueFront, &readyQueueRear);
            processes[processMap[currentPid]].waitingTime += currentTime - processes[processMap[currentPid]].waitingStartTime;
        }

        chart[currentTime] = currentPid;

        if (currentPid != -1) {
            processes[processMap[currentPid]].cpuRemainingTime--;
            if (processes[processMap[currentPid]].cpuRemainingTime == 0) {
                processes[processMap[currentPid]].turnaroundTime = (currentTime + 1 - processes[processMap[currentPid]].arrivalTime);
                currentPid = -1;
                completed++;
            }
        } else {
            chart[currentTime] = 0; // IDLE
        }

        io(waitingQueue, &waitingQueueFront, &waitingQueueRear);

        currentTime++;
    }

    ganttChart(chart, currentTime);
    evaluate();
}

void nonPreemptivePriority(){
    int currentPid = -1;
    int currentTime = 0;

    int chart[1000];

    int readyQueue[MAX_QUEUE_SIZE];
    int waitingQueue[MAX_QUEUE_SIZE];

    int readyQueueFront = 0, readyQueueRear = 0;
    int waitingQueueFront = 0, waitingQueueRear = 0;

    int completed = 0;

    while (completed < processCount) {
        if (currentPid != -1 && processes[processMap[currentPid]].ioBurstTime != 0) {
            if (processes[processMap[currentPid]].ioBurstTiming == processes[processMap[currentPid]].cpuBurstTime - processes[processMap[currentPid]].cpuRemainingTime) {
                enqueue(waitingQueue, &waitingQueueFront, &waitingQueueRear, currentPid);

                for (int i = waitingQueueFront; i != waitingQueueRear; i = (i + 1) % (MAX_QUEUE_SIZE + 1)) {
                    for (int j = (i + 1) % (MAX_QUEUE_SIZE + 1); j != waitingQueueRear; j = (j + 1) % (MAX_QUEUE_SIZE + 1)) {
                        if (processes[processMap[waitingQueue[i]]].ioRemainingTime > processes[processMap[waitingQueue[j]]].ioRemainingTime) {
                            int temp = waitingQueue[i];
                            waitingQueue[i] = waitingQueue[j];
                            waitingQueue[j] = temp;
                        }
                    }
                }

                processes[processMap[currentPid]].waitingStartTime = currentTime;
                currentPid = -1;
            }
        }

        for (int i = 1; i < processCount + 1; i++) {
            if (processes[i].arrivalTime == currentTime) {
                enqueue(readyQueue, &readyQueueFront, &readyQueueRear, processes[i].pid);
                sortReadyQueueByPriority(readyQueue, &readyQueueFront, &readyQueueRear);
            }
        }

        for (int i = waitingQueueFront; i != waitingQueueRear; i = (i + 1) % (MAX_QUEUE_SIZE + 1)) {
            if (processes[processMap[waitingQueue[i]]].ioRemainingTime == 0) {
                int pid = dequeue(waitingQueue, &waitingQueueFront, &waitingQueueRear);
                enqueue(readyQueue, &readyQueueFront, &readyQueueRear, pid);
                sortReadyQueueByPriority(readyQueue, &readyQueueFront, &readyQueueRear);
            }
        }

        if (currentPid == -1 && readyQueueFront != readyQueueRear) {
            currentPid = dequeue(readyQueue, &readyQueueFront, &readyQueueRear);
            processes[processMap[currentPid]].waitingTime += currentTime - processes[processMap[currentPid]].waitingStartTime;
        }

        chart[currentTime] = currentPid;

        if (currentPid != -1) {
            processes[processMap[currentPid]].cpuRemainingTime--;
            if (processes[processMap[currentPid]].cpuRemainingTime == 0) {
                processes[processMap[currentPid]].turnaroundTime = (currentTime + 1 - processes[processMap[currentPid]].arrivalTime);
                currentPid = -1;
                completed++;
            }
        } else {
            chart[currentTime] = 0; // IDLE
        }

        io(waitingQueue, &waitingQueueFront, &waitingQueueRear);

        currentTime++;
    }

    ganttChart(chart, currentTime);
    evaluate();
}

void preemptiveSJF(){
    int currentPid = -1;
    int currentTime = 0;

    int chart[1000];

    int readyQueue[MAX_QUEUE_SIZE];
    int waitingQueue[MAX_QUEUE_SIZE];

    int readyQueueFront = 0, readyQueueRear = 0;
    int waitingQueueFront = 0, waitingQueueRear = 0;

    int completed = 0;

    while (completed < processCount) {
        if (currentPid != -1 && processes[processMap[currentPid]].ioBurstTime != 0) {
            if (processes[processMap[currentPid]].ioBurstTiming == processes[processMap[currentPid]].cpuBurstTime - processes[processMap[currentPid]].cpuRemainingTime) {
                enqueue(waitingQueue, &waitingQueueFront, &waitingQueueRear, currentPid);

                for (int i = waitingQueueFront; i != waitingQueueRear; i = (i + 1) % (MAX_QUEUE_SIZE + 1)) {
                    for (int j = (i + 1) % (MAX_QUEUE_SIZE + 1); j != waitingQueueRear; j = (j + 1) % (MAX_QUEUE_SIZE + 1)) {
                        if (processes[processMap[waitingQueue[i]]].ioRemainingTime > processes[processMap[waitingQueue[j]]].ioRemainingTime) {
                            int temp = waitingQueue[i];
                            waitingQueue[i] = waitingQueue[j];
                            waitingQueue[j] = temp;
                        }
                    }
                }

                processes[processMap[currentPid]].waitingStartTime = currentTime;
                currentPid = -1;
            }
        }

        int isArrivalNow = -1; // isPreemptive flag

        for (int i = 1; i < processCount + 1; i++) {
            if (processes[i].arrivalTime == currentTime) {
                enqueue(readyQueue, &readyQueueFront, &readyQueueRear, processes[i].pid);
                sortReadyQueueByCpuBurstTime(readyQueue, &readyQueueFront, &readyQueueRear);
                isArrivalNow = 1;
            }
        }

        for (int i = waitingQueueFront; i != waitingQueueRear; i = (i + 1) % (MAX_QUEUE_SIZE + 1)) {
            if (processes[processMap[waitingQueue[i]]].ioRemainingTime == 0) {
                int pid = dequeue(waitingQueue, &waitingQueueFront, &waitingQueueRear);
                enqueue(readyQueue, &readyQueueFront, &readyQueueRear, pid);
                sortReadyQueueByCpuBurstTime(readyQueue, &readyQueueFront, &readyQueueRear);
                isArrivalNow = 1;
            }
        }

        if (currentPid == -1 && readyQueueFront != readyQueueRear) {
            currentPid = dequeue(readyQueue, &readyQueueFront, &readyQueueRear);
            processes[processMap[currentPid]].waitingTime += currentTime - processes[processMap[currentPid]].waitingStartTime;
        } else if(currentPid != -1 && isArrivalNow == 1){
            int tempPid = dequeue(readyQueue, &readyQueueFront, &readyQueueRear);
            if(processes[processMap[tempPid]].cpuRemainingTime < processes[processMap[currentPid]].cpuRemainingTime){
                //curpid 여기서 waitingStartTime을 currentTime으로 바꿔야 하나?
                processes[processMap[currentPid]].waitingStartTime = currentTime;
                enqueue(readyQueue, &readyQueueFront, &readyQueueRear, currentPid);
                sortReadyQueueByCpuBurstTime(readyQueue, &readyQueueFront, &readyQueueRear);
                currentPid = tempPid;
                processes[processMap[currentPid]].waitingTime += currentTime - processes[processMap[currentPid]].waitingStartTime;
            }else{
                enqueue(readyQueue, &readyQueueFront, &readyQueueRear, tempPid);
                sortReadyQueueByCpuBurstTime(readyQueue, &readyQueueFront, &readyQueueRear);
            }
        }

        chart[currentTime] = currentPid;

        if (currentPid != -1) {
            processes[processMap[currentPid]].cpuRemainingTime--;
            if (processes[processMap[currentPid]].cpuRemainingTime == 0) {
                processes[processMap[currentPid]].turnaroundTime = (currentTime + 1 - processes[processMap[currentPid]].arrivalTime);
                currentPid = -1;
                completed++;
            }
        } else {
            chart[currentTime] = 0; // IDLE
        }

        io(waitingQueue, &waitingQueueFront, &waitingQueueRear);

        currentTime++;
    }

    ganttChart(chart, currentTime);
    evaluate();
}

void preemptivePriority(){
    int currentPid = -1;
    int currentTime = 0;

    int chart[1000];

    int readyQueue[MAX_QUEUE_SIZE];
    int waitingQueue[MAX_QUEUE_SIZE];

    int readyQueueFront = 0, readyQueueRear = 0;
    int waitingQueueFront = 0, waitingQueueRear = 0;

    int completed = 0;

    while (completed < processCount) {
        if (currentPid != -1 && processes[processMap[currentPid]].ioBurstTime != 0) {
            if (processes[processMap[currentPid]].ioBurstTiming == processes[processMap[currentPid]].cpuBurstTime - processes[processMap[currentPid]].cpuRemainingTime) {
                enqueue(waitingQueue, &waitingQueueFront, &waitingQueueRear, currentPid);

                for (int i = waitingQueueFront; i != waitingQueueRear; i = (i + 1) % (MAX_QUEUE_SIZE + 1)) {
                    for (int j = (i + 1) % (MAX_QUEUE_SIZE + 1); j != waitingQueueRear; j = (j + 1) % (MAX_QUEUE_SIZE + 1)) {
                        if (processes[processMap[waitingQueue[i]]].ioRemainingTime > processes[processMap[waitingQueue[j]]].ioRemainingTime) {
                            int temp = waitingQueue[i];
                            waitingQueue[i] = waitingQueue[j];
                            waitingQueue[j] = temp;
                        }
                    }
                }

                processes[processMap[currentPid]].waitingStartTime = currentTime;
                currentPid = -1;
            }
        }

        int isArrivalNow = -1;

        for (int i = 1; i < processCount + 1; i++) {
            if (processes[i].arrivalTime == currentTime) {
                enqueue(readyQueue, &readyQueueFront, &readyQueueRear, processes[i].pid);
                sortReadyQueueByPriority(readyQueue, &readyQueueFront, &readyQueueRear);
                isArrivalNow = 1;
            }
        }

        for (int i = waitingQueueFront; i != waitingQueueRear; i = (i + 1) % (MAX_QUEUE_SIZE + 1)) {
            if (processes[processMap[waitingQueue[i]]].ioRemainingTime == 0) {
                int pid = dequeue(waitingQueue, &waitingQueueFront, &waitingQueueRear);
                enqueue(readyQueue, &readyQueueFront, &readyQueueRear, pid);
                sortReadyQueueByPriority(readyQueue, &readyQueueFront, &readyQueueRear);
                isArrivalNow = 1;
            }
        }

        if (currentPid == -1 && readyQueueFront != readyQueueRear) {
            currentPid = dequeue(readyQueue, &readyQueueFront, &readyQueueRear);
            processes[processMap[currentPid]].waitingTime += currentTime - processes[processMap[currentPid]].waitingStartTime;
        }
        else if(currentPid != -1 && isArrivalNow == 1){
            int tempPid = dequeue(readyQueue, &readyQueueFront, &readyQueueRear);
            if(processes[processMap[tempPid]].priority < processes[processMap[currentPid]].priority){
                //preSJF랑 같은 위치
                processes[processMap[currentPid]].waitingStartTime = currentTime;
                enqueue(readyQueue, &readyQueueFront, &readyQueueRear, currentPid);
                sortReadyQueueByPriority(readyQueue, &readyQueueFront, &readyQueueRear);
                currentPid = tempPid;
                processes[processMap[currentPid]].waitingTime += currentTime - processes[processMap[currentPid]].waitingStartTime;
            }else{
                enqueue(readyQueue, &readyQueueFront, &readyQueueRear, tempPid);
                sortReadyQueueByPriority(readyQueue, &readyQueueFront, &readyQueueRear);
            }
        }

        chart[currentTime] = currentPid;

        if (currentPid != -1) {
            processes[processMap[currentPid]].cpuRemainingTime--;
            if (processes[processMap[currentPid]].cpuRemainingTime == 0) {
                processes[processMap[currentPid]].turnaroundTime = (currentTime + 1 - processes[processMap[currentPid]].arrivalTime);
                currentPid = -1;
                completed++;
            }
        } else {
            chart[currentTime] = 0; // IDLE
        }

        io(waitingQueue, &waitingQueueFront, &waitingQueueRear);

        currentTime++;
    }

    ganttChart(chart, currentTime);
    evaluate();
}

void roundRobin(int timeQuantum) {
    int currentPid = -1;
    int currentTime = 0;

    int chart[1000];

    int readyQueue[MAX_QUEUE_SIZE];
    int waitingQueue[MAX_QUEUE_SIZE];

    int readyQueueFront = 0, readyQueueRear = 0;
    int waitingQueueFront = 0, waitingQueueRear = 0;

    int completed = 0;
    int timeSlice = 0;

    while (completed < processCount) {
        if (currentPid != -1 && processes[processMap[currentPid]].ioBurstTime != 0) {
            if (processes[processMap[currentPid]].ioBurstTiming == processes[processMap[currentPid]].cpuBurstTime - processes[processMap[currentPid]].cpuRemainingTime) {
                enqueue(waitingQueue, &waitingQueueFront, &waitingQueueRear, currentPid);

                for (int i = waitingQueueFront; i != waitingQueueRear; i = (i + 1) % (MAX_QUEUE_SIZE + 1)) {
                    for (int j = (i + 1) % (MAX_QUEUE_SIZE + 1); j != waitingQueueRear; j = (j + 1) % (MAX_QUEUE_SIZE + 1)) {
                        if (processes[processMap[waitingQueue[i]]].ioRemainingTime > processes[processMap[waitingQueue[j]]].ioRemainingTime) {
                            int temp = waitingQueue[i];
                            waitingQueue[i] = waitingQueue[j];
                            waitingQueue[j] = temp;
                        }
                    }
                }

                processes[processMap[currentPid]].waitingStartTime = currentTime;
                currentPid = -1;
                timeSlice = 0;
            }
        }

        for (int i = 1; i < processCount + 1; i++) {
            if (processes[i].arrivalTime == currentTime) {
                enqueue(readyQueue, &readyQueueFront, &readyQueueRear, processes[i].pid);
            }
        }

        for (int i = waitingQueueFront; i != waitingQueueRear; i = (i + 1) % (MAX_QUEUE_SIZE + 1)) {
            if (processes[processMap[waitingQueue[i]]].ioRemainingTime == 0) {
                int pid = dequeue(waitingQueue, &waitingQueueFront, &waitingQueueRear);
                enqueue(readyQueue, &readyQueueFront, &readyQueueRear, pid);
            }
        }

        if (currentPid == -1 && readyQueueFront != readyQueueRear) {
            currentPid = dequeue(readyQueue, &readyQueueFront, &readyQueueRear);
            processes[processMap[currentPid]].waitingTime += currentTime - processes[processMap[currentPid]].waitingStartTime;
            timeSlice = 0;
        }

        chart[currentTime] = currentPid;

        if (currentPid != -1) {
            processes[processMap[currentPid]].cpuRemainingTime--;
            timeSlice++;

            if (processes[processMap[currentPid]].cpuRemainingTime == 0) {
                processes[processMap[currentPid]].turnaroundTime = (currentTime + 1 - processes[processMap[currentPid]].arrivalTime);
                currentPid = -1;
                completed++;
                timeSlice = 0;
            } else if (timeSlice == timeQuantum) {
                if (processes[processMap[currentPid]].ioBurstTime != 0 && processes[processMap[currentPid]].ioBurstTiming == processes[processMap[currentPid]].cpuBurstTime - processes[processMap[currentPid]].cpuRemainingTime) {
                    io(waitingQueue, &waitingQueueFront, &waitingQueueRear);
                    enqueue(waitingQueue, &waitingQueueFront, &waitingQueueRear, currentPid);
                    for (int i = waitingQueueFront; i != waitingQueueRear; i = (i + 1) % (MAX_QUEUE_SIZE + 1)) {
                        for (int j = (i + 1) % (MAX_QUEUE_SIZE + 1); j != waitingQueueRear; j = (j + 1) % (MAX_QUEUE_SIZE + 1)) {
                            if (processes[processMap[waitingQueue[i]]].ioRemainingTime > processes[processMap[waitingQueue[j]]].ioRemainingTime) {
                                int temp = waitingQueue[i];
                                waitingQueue[i] = waitingQueue[j];
                                waitingQueue[j] = temp;
                            }
                        }
                    }
                    processes[processMap[currentPid]].waitingStartTime = currentTime+1;
                    currentPid = -1;
                    timeSlice = 0;
                    currentTime++;
                    continue;
                        
                } else {
                    enqueue(readyQueue, &readyQueueFront, &readyQueueRear, currentPid);
                    processes[processMap[currentPid]].waitingStartTime = currentTime+1;
                    currentPid = -1;
                    timeSlice = 0;
                }
                
            }
        } else {
            chart[currentTime] = 0; // IDLE
        }

        io(waitingQueue, &waitingQueueFront, &waitingQueueRear);

        currentTime++;
    }

    ganttChart(chart, currentTime);
    evaluate();
}
